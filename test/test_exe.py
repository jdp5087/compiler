import os
import subprocess as sp
import unittest
from warnings import warn

def mycall(*popenargs, write_fd=None, timeout=None, shell=True, **kwargs):
    """This function is a modification of subprocess.check_output.
    I wanted to be able to do a few extra things, such as recieve both
    retcode and output on return, and also be able to write to file and
    recieve output on return. See cpython/Lib/subprocess.py in Python source
    for details.
    """
    if 'stdout' in kwargs:
        raise ValueError('stdout argument not allowed, it will be overridden.' \
                         ' Please supply a writeable file descriptor as keyword' \
                         ' argument write_fd in order to write to file.')
    if 'input' in kwargs:
        inputdata = kwargs['input']
        del kwargs['input']
        kwargs['stdin'] = sp.PIPE
    else:
        inputdata = None
    with sp.Popen(*popenargs, stdout=sp.PIPE, **kwargs) as process:
        try:
            output, unused_err = process.communicate(inputdata, timeout=timeout)
        except TimeoutExpired:
            process.kill()
            output, unused_err = process.communicate()
            raise TimeoutExpired(process.args, timeout, output=output)
        except:
            process.kill()
            process.wait()
            raise
        retcode = process.poll()
    if write_fd is not None:
        try:
            write_fd.write(output)
        except:
            warn('Could not write to file: %s' % (write_fd.name))
        finally:
            write_fd.close()
    return (retcode, output)

class CompileException(Exception): pass
class AssembleException(Exception): pass
class LinkException(Exception): pass

def find_file_or_dir(root, f, file_type="filenames"):
    for dirpath, dirnames, filenames in os.walk(os.path.abspath(root)):
        if f in locals()[file_type]:
            return os.path.join(dirpath, f)
    raise FileNotFoundError("Could not find %s in directory starting at %s" %
                            (f, root))

ORIGINAL_DIRECTORY = os.getcwd()
PROJECT_ROOT = "/home/jdp/mycomp"
COMPILER_NAME = "df"
TEST_DIR = find_file_or_dir(PROJECT_ROOT, "test", "dirnames")
OUTPUT_BASENAME = "asm_test{}"

OUTPUT_PATH = os.path.join(TEST_DIR, OUTPUT_BASENAME.format(".S"))
OUTPUT_OBJECT_PATH = os.path.join(TEST_DIR, OUTPUT_BASENAME.format(".o"))
OUTPUT_EXECUTABLE_PATH = os.path.join(TEST_DIR, OUTPUT_BASENAME.format(""))

CLEANUP_PATHS = [OUTPUT_PATH, OUTPUT_OBJECT_PATH, OUTPUT_EXECUTABLE_PATH]

MAKEFILE_DIR = os.path.dirname(find_file_or_dir(PROJECT_ROOT, "makefile"))
os.chdir(MAKEFILE_DIR)
### MAKE EXECUTABLE ###
if (sp.call(["make", COMPILER_NAME]) != 0):
    raise CompileException("make did not run successfully")
EXECUTABLE_PATH = find_file_or_dir(PROJECT_ROOT, COMPILER_NAME)

class Base:
    def setUp(self, *args, **kwargs):
        """Sets self.cmd to a formattable string which will
        be used to pass the executable source code on its
        standard input.
        """
        self.cmd = EXECUTABLE_PATH
        super().setUp(*args, **kwargs)

    def tearDown(self, *args, **kwargs):
        for path in CLEANUP_PATHS:
            if os.path.exists(path):
                os.remove(path)
        super().tearDown(*args, **kwargs)
    
    def exec_si(self, command, write_file=None):
        if isinstance(command, str):
            try:
                command = bytes(command, encoding='utf-8')
            except:
                raise
        elif isinstance(command, bytes):
            pass
        else:
            raise TypeError("Need Bytes or utf-8 string for input to the" \
                            " process call.")
        if write_file is not None:
            write_fd = open(write_file, 'wb')
        return mycall([self.cmd], write_fd=write_fd, input=command, shell=True)

    def compile_assemble_link(self, command):
        code, output = self.exec_si(command, OUTPUT_PATH)
        if code != 0:
            raise CompileException("Command could not be properly compiled: {}" \
                                   "".format(command))
        if (sp.call(["as", OUTPUT_PATH, "-o", OUTPUT_OBJECT_PATH]) != 0):
            raise AssembleException("Command could not be properly assembled: {}" \
                                    "".format(command))
        if (sp.call(["ld", OUTPUT_OBJECT_PATH, "-o", OUTPUT_EXECUTABLE_PATH]) != 0):
            raise LinkException("Command could not be properly linked: {}" \
                                "".format(command))


class CompilerCommandsTest(Base, unittest.TestCase):

    def test_two_single_digits_add_properly(self):
        self.compile_assemble_link("9+1")
        code = sp.call([OUTPUT_EXECUTABLE_PATH])
        self.assertEqual(code, 10)

    def test_more_complicated_arithmetic(self):
        self.compile_assemble_link("2+(9/(6/2))*8")
        code = sp.call([OUTPUT_EXECUTABLE_PATH])
        self.assertEqual(code, 26)
        



if __name__ == '__main__':
    unittest.main()
