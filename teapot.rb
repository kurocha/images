
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "3.0"

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
	target.depends "Language/C++17"
	
	target.depends "Library/Memory", public: true
	target.depends "Library/Resources", public: true
	
	target.depends "Library/png", public: true
	target.depends "Library/jpeg", public: true
	target.depends "Library/webp", public: true
	
	target.depends "Library/Numerics", public: true
	
	target.provides "Library/Images" do
		source_root = target.package.path + 'source'
		
		library_path = build static_library: "Images", source_files: source_root.glob('Images/**/*.cpp')
		
		append linkflags library_path
		append header_search_paths source_root
	end
end

define_target "images-tests" do |target|
	target.depends 'Library/UnitTest'
	target.depends "Language/C++17"
	
	target.depends 'Library/Images'
	target.depends 'Library/Time'
	
	target.provides "Test/Images" do |*arguments|
		test_root = target.package.path + 'test'
		
		run source_files: test_root.glob('Images/**/*.cpp'), arguments: arguments
	end
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
	
	configuration.require "generate-project"
	configuration.require "generate-travis"
	
	configuration.require "time"
end

define_configuration "images" do |configuration|
	configuration.public!
	
	configuration.require "numerics"
	configuration.require "resources"
	
	configuration.require "zlib-ng"
	
	configuration.require "png"
	configuration.require "jpeg"
	configuration.require "webp"
end
