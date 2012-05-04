Guide To Build Freescale's Linux BSP for the Olinuxino
===

*Thanks to Raivis for the initial description[1] and patches.*

Download these:

* [Linux 2.6.31 Sources](https://www.freescale.com/webapp/Download?colCode=L2.6.31_10.05.00_SDK_SOURCE), md5: dd90e0383d66abb9c9f48e94715ba6d1
* [Patches](http://www.freescale.com/webapp/Download?colCode=L2.6.31_10.05.00_SDK_PATCHES), md5: 564d0fbbd7ca7a999a6b7680019bafc0
* [Documentation](http://www.freescale.com/webapp/Download?colCode=IMX23_EVK10_05_LINUXDOCS_BUNDLE), md5: fadcb3cd37d29139561d7945f469466c

Registration is required for these downloads. You're basically agreeing to not hold Freescale responsible for anything.

Install LTIB
===

Unpack the LTIB bundle:

    $ tar zxf L2.6.31_10.05.02_ER_source.gz

Install it:

    $ cd L2.6.31_10.05.02_ER_source
    $ ./install  

    You are about to install the LTIB (GNU/Linux Target Image Builder)

    Before installing LTIB, you must read and accept the EULA 
    (End User License Agreement) which will be presented next.

    Do you want to continue ? Y|n
    y
    <lots of license gibberish>
    I have read and accept the EULA (yes|no):
    yes

    The LTIB files are extracted from a tar file which includes the
    prefix ltib.  After installation you will find LTIB in:
    /home/foo/L2.6.31_10.05.02_ER_source/ltib

    Where do you want to install LTIB ? (/home/foo/L2.6.31_10.05.02_ER_source)

    /home/foo/opt     <-- This will install ltib as /home/foo/opt/ltib

    <lots of output>
    ltib/bin/Ltibrelease.pm
    ltib/bin/device_table.txt
    ltib/bin/spit_profile
    ltib/bin/mk_pkg_results
    ltib/bin/autobuild_ltib
    ltib/bin/ltib_install
    ltib/bin/Ltibaddsrpms.pm
    Copying packages to /home/foo/opt/ltib/pkgs

    Installation complete, your ltib installation has been placed in
    /home/foo/opt/ltib, to complete the installation:

    cd /home/foo/opt/ltib
    ./ltib

LTIB should now be installed.

Applying LTIB Patches from Freescale
---

Make sure the base directory exist:

    $ sudo mkdir -p /opt/freescale/pkgs
    $ sudo chmod a+rw /opt/freescale/pkgs

Install the paches

    $ tar zxf Linux_1005_patches.gz
    $ cd Linux_1005_patches
    $ cp kernel-2.6.31.spec.in ~/opt/ltib/config/platform/imx
    $ sudo cp kernel_patches/* /opt/freescale/pkgs

Applying LTIB Patches for Olinuxino
---

Make sure the base directory exist:

    $ sudo mkdir -p /opt/freescale/pkgs
    $ sudo chmod a+rw /opt/freescale/pkgs

This patch adds support for the Olinuxino board to LTIB:

    $ cat patches/01-ltib.patch | (cd ~/opt/ltib && patch -p1)
    $ cp patches/01-olinuxino.patch /opt/freescale/pkgs
    $ cp patches/01-bootlets.patch /opt/freescale/pkgs

The LTIB version used is a bit old so it doesn't properly detect the required
libraries for LTIB to build so this patch will bring in some changes from
LTIB's main repository:

    $ cat patches/02-ltib-Ltibutils.patch | (cd ~/opt/ltib && patch -p1)

### Optional Patches

Apply these patches if you're having problems running ./ltib -m config.

#### WGET

If you're having problems building because of SSL relates issues with wget apply this patch:

    $ cat patches/03-ltib-wget.patch | (cd ~/opt/ltib && patch -p1)

#### Sparse

Apply two patches, one to ltib itself and one to the generated repository:

    $ cat patches/04-ltib-sparse.patch | (cd ~/opt/ltib && patch -p1)
    $ cp patches/sparse-0.4-1316623824.patch /opt/freescale/pkgs

Taken from <http://lists.gnu.org/archive/html/ltib/2011-09/msg00057.html>.

#### Lkc

    $ cat patches/05-ltib-lkc.patch | (cd ~/opt/ltib && patch -p1)

Taken from <http://cvs.savannah.gnu.org/viewvc/ltib/dist/lfs-5.1/lkc/lkc.spec?revision=1.2&root=ltib&view=markup>

#### Git

I had to unset a few Perl-related environment variables, if not git got upset:

    $ unset PERL5LIB PERL_MB_OPT PERL_LOCAL_LIB_ROOT PERL_MM_OPT

See <http://marinp.blogs.uv.es/2009/12/30/perl-locallib-problems-with-some-makefiles-eg-git-manual-installtion/>.

Configuring LTIB
===

'Configuring' is probably not the right word, but anyway. This step will build a lot of packages for your host machine and run you through a set of configuration dialogs to configure the kernel and root image.

    $ cd ~/opt/ltib
    $ ./ltib -m config

If you're getting an error message saying that you're missing some host packages, you need to install those. The patches above should have fixed any detection problems related to running ltib on recent OSes.

Building
===

After you've successfully ran ./ltib -m config you're ready to build the image. Simply execute:

    $ cd ~/opt/ltib
    $ ./ltib

and wait for a while. If everything is working as expected you should have a complete build image ready.

    sudo /opt/freescale/ltib/usr/bin/rpm --root /home/foo/opt/ltib/rootfs --dbpath /var/lib/rpm -e --allmatches --nodeps --define '_tmppath /tmp/ltib' modeps 2>/dev/null
    sudo /opt/freescale/ltib/usr/bin/rpm --root /home/foo/opt/ltib/rootfs --dbpath /var/lib/rpm --prefix / --ignorearch -ivh --force --excludedocs --define '_tmppath /tmp/ltib' /home/foo/opt/ltib/rpm/RPMS/arm/modeps-1.0-1.arm.rpm
    Preparing...                ########################################### [100%]
       1:modeps                 ########################################### [100%]

    Processing deployment operations
    ==================================

    Started: Fri May  4 18:24:16 2012
    Ended:   Fri May  4 18:35:43 2012
    Elapsed: 687 seconds

    Build Succeeded

Patches
===

Summary of all available patches:

* [Bootlets patch](./patches/01-bootlets.patch)
* [LTIB patch](./patches/01-ltib.patch)
* [LTIB Ubuntu 11.10 32bit patch](./patches/01-ltib_ubuntu.patch) - not used
* [LTIB patch to bring in support for newer platforms](./patches/02-ltib-Ltibutils.patch)
* [LTIB patch to remove ssl support for wget](./patches/03-ltib-wget.patch)
* [LTIB patch to fix sparse, spec patch](./patches/04-ltib-sparse.patch), [LTIB patch to fix sparse, source patch](patches/sparse-0.4-1316623824.patch)
* [LTIB patch to fix lkc](./patches/05-ltib-lkc.patch)
* [Linux 2.6.31 kernel patch to support OLinuXino](./patches/01-olinuxino.patch)

See Also
===

* [Freescale's i.MX233 home page](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=i.MX233)

[1]: http://tech.groups.yahoo.com/group/olinuxino/message/235
