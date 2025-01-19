from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class casajsonRecipe(ConanFile):
    name = "casajson"
    version = "1.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "Kamil Wisniewski thiro@poczta.onet.pl"
    url = "https://github.com/thirogit/bk3rdparty"
    description = "JSON serialization/deserialization library"
    topics = ("json")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "src/*", "include/*"
    
    def requirements(self):
        self.requires("boost/1.83.0")
        

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["casajson"]

