from conans import ConanFile, CMake, tools

class ThrowingPtrConan(ConanFile):
    """Conan package for throwing_ptr"""
    name = "throwing_ptr"
    version = "0.1.0"
    author = "rockdreamer"
    license = "Boost"
    url = "https://github.com/rockdreamer/throwing_ptr"
    description = "shared_ptr that throws on dereference if null"
    exports_sources = "include/*", "CMakeLists.txt", "tests/*.cpp"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"

    build_requires = "gtest/1.8.0@rockdreamer/testing"
    default_options = "gtest:shared=False"

    def build(self): # this is not building a library, just tests
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
