This is the GrainDB Python package

## Default installation

You would normally install the GrainDB released version using `pip` as follows:
    pip install graindb

## Installing locally

For development, you may need a GrainDB python package that is installed from source.
Proceed as follows.

Set the prefix path:

    export GRAINDB_PREFIX=/path/to/install/graindb

Set the `PYTHONPATH` to the `site-packages` directory under the prefix path;
this will usually work but do check the resulting `PYTHONPATH`.

    export PYTHONPATH=${PYTHONPATH:+${PYTHONPATH}:}$(pip show six | \
      grep "Location:" | cut -d " " -f2 | \
      sed -e "s|/usr|${GRAINDB_PREFIX}|")

### Setup for development

Install the package from the root of the GrainDB reposity:

    cd ../..
    pip3 install --prefix $GRAINDB_PREFIX -e tools/pythonpkg

This creates a package that uses the files in `tools/pythonpkg`, the
best option during development.

### Setup for cloud storage

Alternatively, you may need the package files to reside under the same
prefix where the library is installed; e.g., when installing to cloud
storage from a notebook.

First, get the repository based version number and extract the source distribution.

    cd tools/pythonpkg
    export SETUPTOOLS_SCM_PRETEND_VERSION=$(python setup.py --version)
    python setup.py sdist
    cd ../..
 
Next, copy over the python package related files, and install the package.

    mkdir -p $GRAINDB_PREFIX/src/graindb-pythonpkg
    tar --directory=$GRAINDB_PREFIX/src/graindb-pythonpkg -xzpf tools/pythonpkg/dist/graindb-${SETUPTOOLS_SCM_PRETEND_VERSION}.tar.gz
    pip3 install --prefix $GRAINDB_PREFIX -e $GRAINDB_PREFIX/src/graindb-pythonpkg/graindb-${SETUPTOOLS_SCM_PRETEND_VERSION}
