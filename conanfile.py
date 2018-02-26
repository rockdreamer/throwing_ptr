"""throwing_ptr"""
from conans import ConanFile, CMake

class ThrowingPtrConan(ConanFile):
    """Conan package for throwing_ptr"""
    name = "throwing_ptr"
    version = "0.1.0"
    author = "rockdreamer"
    license = "Boost"
    url = "https://github.com/rockdreamer/throwing_ptr"
    description = "shared_ptr that throws on dereference if null"
    exports_sources = "*"
    settings = "os", "compiler", "arch"
    generators = "cmake"

    build_requires = "Catch/1.9.6@bincrafters/stable"

    def build(self): # this is not building a library, just tests
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
