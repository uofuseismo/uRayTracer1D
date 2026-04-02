from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import cmake_layout, CMake, CMakeDeps, CMakeToolchain

class uRayTracer1D(ConanFile):
   name = "uRayTracer1D"
   license = "MIT"
   description = "Traces rays in a 1D layered medium."
   url = "https://github.com/uofuseismo/uLayerTracer1D"
   settings = "os", "compiler", "build_type", "arch"
   options = {"build_tests" : [True, False]}
   default_options = {"hwloc/*:shared": "True",
                      "spdlog/*:header_only" : "True",
                      "build_tests" : "True",
                     }
   export_sources = "CMakeLists.txt", "LICENSE", "README.md", "cmake/*", "src/*", "testing/*"
   generators = "CMakeDeps", "CMakeToolchain"

   def requirements(self):
       # dependencies
       self.requires("onetbb/2022.3.0")
       self.requires("boost/1.89.0")

   def build_requirements(self):
       # test dependncies and build tools
       self.test_requires("catch2/3.13.0")

   def layout(self):
       # defines the project layout
       cmake_layout(self)

   def build(self):
       # invokes the build system
       cmake = CMake(self)
       cmake.configure()
       cmake.build()

   def test(self):
       if can_run(self):
          cmake.test()

   def package(self):
       # copies files from the build to package folder
       cmake = CMake(self)
       cmake.install()

   def package_info(self):
       self.cpp_info.libs = ["uLayerTracer1D"]

