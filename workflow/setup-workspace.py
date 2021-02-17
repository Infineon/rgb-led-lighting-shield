import argparse, json, os, sys, subprocess

lib_root      = '..'
lib_core_dir  = os.path.join(lib_root, 'src', 'corelib')
lib_pal_dir   = os.path.join(lib_root, 'src', 'pal')
lib_frmwk_dir = os.path.join(lib_root, 'src', 'framework')
lib_conf_dir  = os.path.join(lib_root, 'src', 'config')
lib_test_dir  = os.path.join(lib_root, 'test')
lib_bbtest_dir= os.path.join(lib_test_dir, 'blackbox')
wf_dir        = os.path.join(lib_root, 'workflow')

def arduino_library_path_default():
    '''
    Returns the default Arduino sketchbook path
    (Where libraries are installed by default)
    '''

    ard_sb_path = ""
    if sys.platform == "linux" or sys.platform == "linux2":
        # linux
        pass
    elif sys.platform == "darwin":
        # OS X
        pass
    elif sys.platform == "win32" or sys.platform == "cygwin":
        ard_sb_path = os.path.join('C:\\Users',os.getenv('USERNAME'),'Documents\\Arduino\\libraries')
        
    return ard_sb_path

def symlink_custom(src, dest):
    '''
    Symbolic link calling directly to the OS command
    '''
    if sys.platform == "linux" or sys.platform == "linux2":
        # linux
        pass
    elif sys.platform == "darwin":
        # OS X
        pass
    elif sys.platform == "win32" or sys.platform == "cygwin":
       
        if os.path.isdir(src):
            opt = '/J'
            dest = os.path.join(dest,os.path.basename(src))
        elif os.path.isfile(src):
            opt = '/h'
            dest = os.path.join(dest,os.path.basename(src))

        #try:
        print("src:" + src)
        print("dest: " + dest)
        proc = subprocess.Popen(["mklink", opt, dest, src], shell=True)
        # output, err = proc.communicate()
        proc.wait()
        # output = str(output)
        # print(output)
        #except:
         #   print("Error: WIN32 mklink")

def get_local_conf():
    '''
    Get local workspace configuration 
    '''
    conf_serial = "{}"
    conf_file   = os.path.join(wf_dir, "local.json")

    if os.path.exists(conf_file):
        print("\"local.json\" configuration file found ")
        with open(conf_file) as file:
            conf_serial = json.load(file)
            print(conf_serial)
        file.close()

    return conf_serial

def get_config_sources():
    '''
    Gets all configuration files as an array.
    '''
    src_list = [os.path.join(lib_conf_dir, f) for f in os.listdir(lib_conf_dir)]

    return src_list

def get_corelib_pal_sources():
    '''
    Gets all corelib and pal files as an array.
    '''
    corelib_src_list = [os.path.join(lib_core_dir, f) for f in os.listdir(lib_core_dir)]
    pal_src_list = [os.path.join(lib_pal_dir, f) for f in os.listdir(lib_pal_dir)]
    src_list = corelib_src_list + pal_src_list

    return src_list


def get_frwk_pal_sources(frwk):
    '''
    Gets the framework pal implementation files as an array.
    '''
    src_list = []
    frwk_pal_src_dir = os.path.join(lib_frmwk_dir, frwk, "pal")
    if os.path.exists(frwk_pal_src_dir):
        src_list = [os.path.join(frwk_pal_src_dir, f) for f in os.listdir(frwk_pal_src_dir)]
    else:
        print("Error: Framework does not exists.")
    return src_list


def get_frwk_wrapper_sources(frwk):
    '''
    Gets the framework wrapper files as an array.
    '''
    src_list = []
    frwk_wrap_src_dir = os.path.join(lib_frmwk_dir, frwk, "wrapper")
    if os.path.exists(frwk_wrap_src_dir):
        src_list = [os.path.join(frwk_wrap_src_dir, f) for f in os.listdir(frwk_wrap_src_dir)]
    else:
        print("Error: Framework does not exists.")
    return src_list

def get_bbtest_sources():
    '''
    Gets the blackbox test sources as an array.
    '''
    src_list = [os.path.join(lib_bbtest_dir, f) for f in os.listdir(lib_bbtest_dir)]
    return src_list

def get_frwk_metadata(frwk):
    '''
    Gets the framework metatada files as an array.
    All files located in the framework root folder.
    '''
    meta_list = []
    metadata_dir = os.path.join(lib_frmwk_dir, frwk)
    if os.path.exists(metadata_dir):
        meta_list = [os.path.join(metadata_dir,f) for f in os.listdir(metadata_dir) if os.path.isfile(os.path.join(metadata_dir, f))]
    else:
        print("Error: Framework does not exists.")
    return meta_list

def get_frwk_examples(frwk, examples_dir_name):
    '''
    Gets the framework examples files as an array.
    All files located in the framework root folder.

    arg: examples_dir_name. The examples directory name.
    '''
    examples_list = []
    frwk_examples_dir = os.path.join(lib_frmwk_dir, frwk, examples_dir_name)
    if os.path.exists(frwk_examples_dir):
        examples_dir_list = [os.path.join(frwk_examples_dir,f) for f in os.listdir(frwk_examples_dir)]
        for l in examples_dir_list:
            if os.path.isdir(l):
                examples_src_in_dir_list = [os.path.join(l,f) for f in os.listdir(l) if os.path.isfile(os.path.join(l, f))]
                example_dir = [os.path.basename(l), examples_src_in_dir_list]
                examples_list.append(example_dir)
            elif os.path.isfile(l):
                example_dir = [examples_dir_name, l]
                examples_list.append(example_dir)
    else:
        print("Error: Framework does not exists.")
    return examples_list


def symlink_files(dest, file_list):
    '''
    Create symlinks of the provided list of files in
    the specified destination folder
    '''
    print(dest)
    print(file_list)

    for f in file_list:
        if not os.path.exists(os.path.join(dest, os.path.basename(f))):
            symlink_custom(f,dest)


def cp_files(src, dest, file_list):
    '''
    Copy the provided list of files in the 
    specified destination folder 
    '''
    pass

def arduino_lib_path_from_local_json():
    ''' 
    Gets arduino library installation path
    from local.json configuration file.
    Returns "None" if file or tag does not exist.
    ''' 
    path = None

    local_conf_json = get_local_conf()
    if 'arduino_lib_path' in local_conf_json:
        path = local_conf_json['arduino_lib_path']

    return path

def arduino_make_lib_source_tree(path, lib_dir_name):
    '''
    Create Arduino Library Folder Source tree
    in the given path arduino lib instalation path
    and the library directory name
    '''
    lib_dir = os.path.join(path,lib_dir_name)
    lib_src_dir = os.path.join(path, lib_dir_name, 'src')
    lib_examples_dir = os.path.join(path, lib_dir_name, 'examples')

    dirs = [lib_dir, lib_src_dir, lib_examples_dir]
    for d in dirs:
        if not os.path.exists(d):
            os.makedirs(d)

def arduino_make_lib_example_source_tree(path, lib_dir_name, examples_dir_names):
    ''' 
    Create the Arduino Library source tree inside the 
    Example folder in the given path arduino lib instalation path
    and the library directory name.
    '''
    for d in examples_dir_names:
        lib_example_dir = os.path.join(path, lib_dir_name, 'examples', d)
        if not os.path.exists(lib_example_dir):
            os.makedirs(lib_example_dir)

def arduino_workspace_setup(path):
    '''
    Setup workspace for Arduino
    ''' 
    if os.path.exists(path):

        lib_dir_name = "rgb-dali-shield"

        # Create target lib source tree
        arduino_make_lib_source_tree(path, lib_dir_name)

        # Create symbolic link of configuration files in  "config folder
        config_src = get_config_sources()
        symlink_files(os.path.join(path, lib_dir_name, "src"), config_src)

        # Create symbolic link of corelib and pal files in "src " folder
        corelib_src = get_corelib_pal_sources()
        symlink_files(os.path.join(path, lib_dir_name, "src"), corelib_src)

        # Create symbolic link of arduino pal flles in "src" folder
        ard_pal_src = get_frwk_pal_sources('arduino')
        symlink_files(os.path.join(path, lib_dir_name, "src"), ard_pal_src)

        # Create symbolic link of wrapper files in "src" folder
        ard_wrap_src = get_frwk_wrapper_sources('arduino')
        symlink_files(os.path.join(path, lib_dir_name, "src"), ard_wrap_src)

        # Create symbolic link of each example folder in "examples" folder
        ard_examples_src = get_frwk_examples('arduino','examples')
        examples_dir_names = [d[0] for d in ard_examples_src]
        arduino_make_lib_example_source_tree(path, lib_dir_name, examples_dir_names)
        for f in ard_examples_src:
            symlink_files(os.path.join(path, lib_dir_name, "examples", f[0]), f[1])

        # # Create symbolic link of black box tests in "test/blackbox" folder
        # ard_bbtest_src = get_bbtest_sources()
        # symlink_files(os.path.join(path, lib_dir_name, "src"), ard_bbtest_src)

        # Create symbolic link of metadata files in rootfolder
        ard_metadata = get_frwk_metadata('arduino')
        symlink_files(os.path.join(path, lib_dir_name), ard_metadata)

    else:
        print("Error: Path does not exist")

def cmdline_parser_workspace_setup():
    '''
    Cmdline tool for workspace setup
    '''
    def noarg(args):
        parser.print_help()

    def ws_setup(args):
        if args.framework == 'arduino':
            # Ardiuno lib path from local configuration
            if args.lib_path is None:
                # path = arduino_lib_path_from_local_json()
                path = arduino_library_path_default()
            # Ardiuno lib path passed by argument
            else:
                path = args.lib_path

            if path is not None:
                arduino_workspace_setup(path)
            else:
                print("Add path to \"local.json\" configuration file or pass it by argument.")
        else:
            print("Framework not supported")

            
    parser     = argparse.ArgumentParser(description="PAS CO2 Software Development Framework Workspace Setup Helper Tool")
    subparsers = parser.add_subparsers()
    parser.set_defaults(func=noarg)

    parser_wssetup = subparsers.add_parser('wssetup', help='Framework Workspace Setup')
    parser_wssetup.add_argument('framework', choices=['arduino'], help='Development framework')
    parser_wssetup.add_argument('--lib_path', type=str, help='Library installation path')
    parser_wssetup.set_defaults(func=ws_setup)
 
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    cmdline_parser_workspace_setup()
    # ard_examples_src = get_frwk_examples('arduino','examples')
    # dirs = [d[0] for d in ard_examples_src]
    # print(dirs)