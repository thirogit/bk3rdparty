from conan import ConanFile
from conan.tools.files import copy

class Utf8Conan(ConanFile):
    name = "utf8"
    version = "0.1"
    # No settings/options are necessary, this is header only
    exports_sources = "include/*"
    no_copy_source = True
    
    # Optional metadata
    author = "Nemanja Trifunovic"
    
    def package(self):
        # This will also copy the "include" folder
        copy(self, "*.h", self.source_folder, self.package_folder)

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []