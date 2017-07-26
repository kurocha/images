
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "2.0"

# Project Metadata

define_project "images" do |project|
	project.title = "Images"
	project.summary = 'Load and save images using pixel buffers.'
	
	project.add_author "Samuel Williams"
	
	project.license = "MIT License"
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	
	project.version = "1.0.0"
end

# Build Targets

define_target 'images-library' do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		copy headers: source_root.glob('Images/**/*.{h,hpp}')
		
		build static_library: "Images", source_files: source_root.glob('Images/**/*.cpp')
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends "Language/C++14", private: true
	
	target.depends "Library/Memory"
	target.depends "Library/Resources"
	
	target.depends "Library/png"
	target.depends "Library/jpeg"
	target.depends "Library/webp"
	
	target.depends "Library/Euclid"
	
	target.provides "Library/Images" do
		append linkflags [
			->{install_prefix + 'lib/libImages.a'},
		]
	end
end

define_target "images-tests" do |target|
	target.build do |*arguments|
		test_root = target.package.path + 'test'
		
		copy test_assets: test_root.glob('**/fixtures/**/*')
		
		run tests: 'Images', source_files: test_root.glob('Images/**/*.cpp'), arguments: arguments
	end
	
	target.depends "Library/UnitTest"
	target.depends 'Library/Images'
	
	target.depends "Language/C++14", private: true
	
	target.provides 'Test/Images'
end

# Configurations

define_configuration "development" do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	configuration.import "images"
	
	# Provides all the build related infrastructure:
	configuration.require "platforms"
	configuration.require "build-cmake"
	configuration.require "build-make"
	
	# Provides unit testing infrastructure and generators:
	configuration.require "unit-test"
	
	# Provides some useful C++ generators:
	configuration.require "generate-cpp-class"
	
	configuration.require "platforms"
	
	configuration.require "generate-project"
	configuration.require "generate-travis"
end

define_configuration "images" do |configuration|
	configuration.public!
	
	configuration.require "euclid"
	configuration.require "resources"
	
	configuration.require "png"
	configuration.require "jpeg"
	configuration.require "webp"
end
