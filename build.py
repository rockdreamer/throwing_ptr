"""multi packager for throwing_ptr"""
from conan.packager import ConanMultiPackager

if __name__ == "__main__":
    builder = ConanMultiPackager()
    builder.add_common_builds(pure_c=False)
    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["compiler"] == "Visual Studio":
            filtered_builds.append([settings, options, env_vars, build_requires])
        else:
            env_vars['CXX_STANDARD'] = '11'
            filtered_builds.append([settings, options, env_vars, build_requires])
            env_vars['CXX_STANDARD'] = '14'
            filtered_builds.append([settings, options, env_vars, build_requires])
            env_vars['CXX_STANDARD'] = '17'
            filtered_builds.append([settings, options, env_vars, build_requires])
    builder.builds = filtered_builds
    builder.run()
