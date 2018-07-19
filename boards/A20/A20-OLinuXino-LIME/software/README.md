# Python packages for OLinuXino boards

**Before using the packages make sure that the pins you're will be using are not already used by another peripheral!**

You could use package that is not targeted from your board but this is not recommended.

## Source

[pyA20Lime](https://pypi.python.org/pypi/pyA20Lime)

## Installing from pypi

**You will need python-dev or python3-dev to build packages.**

Download tarball from pypi.python.org. Extract it and istall it:

```bash
tar -zxvf <package_name>.tar.gz
cd <package_name>
sudo python setup.py install
```

## Installing from git repository

**You will need python-dev or python3-dev to build packages.**

Download the folder for your board.

**If you want to download single folder svn is needed**

```bash
svn export https://github.com/OLIMEX/OLINUXINO/trunk/A20/A20-OLinuXino-LIME/software/
cd pyA20Lime
python setup.py install
```

