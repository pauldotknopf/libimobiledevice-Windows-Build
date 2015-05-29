Framework '4.5.1'

properties {
    $base_dir = resolve-path .
    $libimobiledevice_source_dir = "$base_dir\libimobiledevice-source"
    $libiconv_source_dir = "$base_dir\iconv-source"
    $libplist_source_dir = "$base_dir\libplist-source"
    $configuration = "Release"
}

task default -depends clean, checkout, build

task clean {
    delete_directory $libimobiledevice_source_dir
    delete_directory $libiconv_source_dir
    delete_directory $libplist_source_dir
}

task checkout {
    exec { git clone git://git.savannah.gnu.org/libiconv.git $libiconv_source_dir }
    exec { git clone https://github.com/libimobiledevice/libimobiledevice.git $libimobiledevice_source_dir }
    exec { git clone https://github.com/libimobiledevice/libplist.git $libplist_source_dir  }
    
}

task build {

}

function global:delete_directory($directory_name) {
    rd $directory_name -recurse -force  -ErrorAction SilentlyContinue | out-null
}

function global:create_directory($directory_name) {
    if (!(Test-Path -path $directory_name)) { new-item $directory_name -force -type directory}
}