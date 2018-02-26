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
    settings = "os", "compiler"
    generators = "cmake"

    build_requires = "catch2/2.1.2@bincrafters/stable"

    def build(self): # this is not building a library, just tests
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
