from distutils.core import setup
from distutils.core import Extension
from distutils.command.build_ext import build_ext as _build_ext

import sys

processor_type = "sun7i"

try:
    input = raw_input
except NameError:
    pass


def print_color(text):
    """
    Print text in yellow :)
    :param text: String to be colored
    :return: Colored text
    """

    return '\033[0;33m' + text + '\033[0m'


def print_warning():
    """
    Print confirmation dialog
    :return:
    """
    print (print_color("Warning! ") + "Detected and target processor mismatch. ")


    var = input("Do you want to continue [Y/n]? ")
    if var == 'Y' or var == 'y':
        return
    else:
        print ("Abort.")
        sys.exit(1)


def check_processor():
    """
    Detect processor type
    :return:
    """
    cpuinfo = open("/proc/cpuinfo", 'r')
    for line in cpuinfo:
        if "Hardware" in line:
            processor = line.split(":")[1].rstrip()

            if "sun4i" in processor:
                print ("Detected processor: " + print_color(processor) + " (Probably Allwinner A10)")

            elif "sun5i" in processor:
                print ("Detected processor: " + print_color(processor) + " (Probably Allwinner A13)")

            elif "sun7i" in processor:
                print ("Detected processor: " + print_color(processor) + " (Probably Allwinner A20)")

            else:
                print ("Detected processor: " + print_color("unknown"))


            if processor_type not in processor:
                print_warning()

            return

    print ("No processor detected")
    print_warning()


class build_ext(_build_ext):
    def run(self):
        check_processor()
        _build_ext.run(self)


modules = [
    Extension('pyA20.gpio.gpio', sources=['pyA20/gpio/gpio_lib.c', 'pyA20/gpio/gpio.c']),

    Extension('pyA20.i2c', sources=['pyA20/i2c/i2c_lib.c', 'pyA20/i2c/i2c.c']),

    Extension('pyA20.spi', sources=['pyA20/spi/spi_lib.c', 'pyA20/spi/spi.c']),

    Extension('pyA20.gpio.connector', sources=['pyA20/gpio/connector/connector.c']),

    Extension('pyA20.gpio.port', sources=['pyA20/gpio/port/port.c']),

]

setup(
    name='pyA20',
    version='0.2.1',
    author='Stefan Mavrodiev',
    author_email='support@olimex.com',
    url='https://www.olimex.com/',
    license='MIT',
    packages=['pyA20', 'pyA20.gpio'],
    description='Control GPIO, I2C and SPI',
    long_description=open('README.txt').read() + open('CHANGES.txt').read(),
    classifiers=['Development Status :: 3 - Alpha',
                 'Environment :: Console',
                 'Intended Audience :: Developers',
                 'Intended Audience :: Education',
                 'License :: OSI Approved :: MIT License',
                 'Operating System :: POSIX :: Linux',
                 'Programming Language :: Python',
                 'Topic :: Home Automation',
                 'Topic :: Software Development :: Embedded Systems'
    ],
    ext_modules=modules,
    cmdclass={'build_ext': build_ext}
)
