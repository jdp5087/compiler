import os
import subprocess
import unittest

PROJECT_ROOT = "/home/jdp/mycomp"
COMPILER_NAME = "df"

def find_file(root, f):
    for dirpath, dirnames, filenames in os.walk(os.path.abspath(root)):
        if f in filenames:
            return os.path.join(dirpath, f)
    raise FileNotFoundError('Could not find %s in directory starting at %s' %
                            (f, root))

EXECUTABLE_PATH = find_file(PROJECT_ROOT, COMPILER_NAME)

class Base:
    def setUp(self, *args, **kwargs):
        """Sets self.cmd to a formattable string which will
        be used to pass the executable source code on its
        standard input.
        """
        self.cmd = 'echo {} | ' + EXECUTABLE PATH
        super().setUp(*args, **kwargs)


class CompilerCommandsTest(Base, unittest.TestCase):

    def test_two_single_digits_add_properly(self):
        cmd = '2+1'
        code, output = subprocess.getstatusoutput(self.cmd.format(cmd))
        self.assertEqual(code, 0)
        self.assertEqual(output, 3)

if __name__ == '__main__':
    unittest.main()
