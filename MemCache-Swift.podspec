Pod::Spec.new do |s|
    s.name             = 'MemCache-Swift'
    s.version          = '0.0.1'
    s.summary          = 'A caching library for Swift.'
    s.homepage         = 'https://github.com/your-username/MemCache-Swift'
    s.license          = { :type => 'MIT', :file => 'LICENSE' }
    s.author           = { 'Your Name' => 'your-email@example.com' }
    s.source           = { :git => 'https://github.com/panicfrog/MemCache.git', :tag => s.version.to_s }
    s.source_files     = 'include/**/*.{h,hpp}', 'src/**/*.{swift}'
    s.public_header_files = 'include/**/*.h'
    s.swift_version    = '5.7'
    s.requires_arc     = true
    s.header_mappings_dir = 'include'
    s.ios.deployment_target = '11.0'
    s.vendored_frameworks = 'install-ios/MemCache.xcframework'
  end
