from conan import ConanFile
from conan.tools.files import copy

class TypeStringConan(ConanFile):
    name = "typestring"
    version = "0.1"
    # No settings/options are necessary, this is header only
    exports_sources = "include/*"
    no_copy_source = True
    
    # Optional metadata
    license = "Mozilla Public Licence 2.0"
    author = "George Makrydakis"
    url = "https://github.com/irrequietus/clause"
    description = "seamlessly define strings as types in situ within a template parameter list as a language feature."
    topics = ("situ", "template", "type")

    def package(self):
        # This will also copy the "include" folder
        copy(self, "*.h", self.source_folder, self.package_folder)

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []