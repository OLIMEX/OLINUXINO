Python packages for OLinuXino boards
==============

**Before using the packages make sure that the pins you're will be using are
not already used by another periphery!**

You could use package that is not targeted from your board but this is not
recommended.


Available packages
--------------

- pyA10Lime	-	https://pypi.python.org/pypi/pyA10Lime
- pyA13		-	https://pypi.python.org/pypi/pyA13
- pyA13SOM	-	https://pypi.python.org/pypi/pyA13SOM
- pyA20		-	https://pypi.python.org/pypi/pyA20
- pyA20EVB	-	https://pypi.python.org/pypi/pyA20EVB
- pyA20Lime	-	https://pypi.python.org/pypi/pyA20Lime
- pyA20Lime2	-	https://pypi.python.org/pypi/pyA20Lime2
- pyA20SOM	-	https://pypi.python.org/pypi/pyA20SOM


Installing from pypi
--------------

**You will need python-dev or python3-dev to build packages**

Download tarball from pypi.python.org. Extract it and istall it:

	# tar -zxvf <package_name>.tar.gz
	# cd <package_name>
	# sudo python setup.py install


Installing from github
--------------

**You will need python-dev or python3-dev to build packages**

Download the folder for your board.

*If you want to download single folder svn is needed*


	# svn export https://github.com/OLIMEX/OLINUXINO/trunk/SOFTWARE/Python/
	# cd pyA10Lime
	# python setup.py install
 

