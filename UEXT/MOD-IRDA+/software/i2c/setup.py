from distutils.core import setup, Extension
         
                        
setup(
    name                = 'I2C',
    version             = '0.1.1',
    author              = 'Stefan Mavrodiev',
    author_email        = 'support@olimex.com',
    url                 = 'https://www.olimex.com/',
    license             = 'MIT',
    description         = 'Alternative I2C library',
    long_description    = open('README.txt').read(),
    classifiers         = [ 'Development Status :: 3 - Alpha',
                            'Environment :: Console',
                            'Intended Audience :: Developers',
                            'Intended Audience :: Education',
                            'License :: OSI Approved :: MIT License',
                            'Operating System :: POSIX :: Linux',
                            'Programming Language :: Python',
                            'Topic :: Home Automation',
                            'Topic :: Software Development :: Embedded Systems'
          ],
    ext_modules         = [Extension('I2C', ['source/pyI2C.c', 'source/i2c.c'])],
    package_dir={'': 'source'},
    packages=[''],
  
                            
)
