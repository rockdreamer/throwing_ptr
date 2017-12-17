"""multi packager for throwing_ptr"""
from conan.packager import ConanMultiPackager
from distutils.version import LooseVersion

def standards_for_compiler(compiler, compiler_version):
    parsed_version = LooseVersion(compiler_version)
    if compiler == 'apple-clang':
        if parsed_version >= LooseVersion('8.0'):
            return ['11', '14', '17']
        else:
            return ['11', '14']
    elif compiler == 'clang':
        if parsed_version >= LooseVersion('5.0'):
            return ['11', '14', '17']
        elif parsed_version >= LooseVersion('3.9'):
            return ['11', '14']
        else:
            return ['11']
    elif compiler == 'gcc':
        if parsed_version >= LooseVersion('7'):
            return ['11', '14', '17']
        elif parsed_version >= LooseVersion('5'):
            return ['11', '14']
        else:
            return ['11']
    elif compiler == 'Visual Studio':
        if parsed_version >= LooseVersion('15'):
            return ['11', '14', '17']
        elif parsed_version >= LooseVersion('14'):
            return ['11', '14']
        else:
            return ['11']
    else:
        return ['11']

if __name__ == "__main__":
    builder = ConanMultiPackager()
    builder.add_common_builds(pure_c=False)
    filtered_builds = []
    for settings, options, env_vars, build_requires in builder.builds:
        if settings["build_type"] == "Debug":
            continue
        if settings["compiler"] == "Visual Studio":
            filtered_builds.append([settings, options, env_vars, build_requires])
        else:
            for std in standards_for_compiler(settings["compiler"], settings["compiler.version"]):
                env_vars = env_vars.copy()
                env_vars['CXX_STANDARD'] = std
                filtered_builds.append([settings, options, env_vars, build_requires])
    builder.builds = filtered_builds
    builder.run()
