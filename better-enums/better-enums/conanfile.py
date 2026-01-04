from conan import ConanFile
from conan.tools.files import copy


class BetterEnumsConan(ConanFile):
    name = "better-enums"
    version = "0.1"
    # No settings/options are necessary, this is header only
    exports_sources = "*.h"
    # We can avoid copying the sources to the build folder in the cache
    no_copy_source = True

    def package(self):
        copy(self, "enum.h", self.source_folder, self.package_folder + "/include/better-enums")

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []