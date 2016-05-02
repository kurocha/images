# Dream Imaging

Dream Imaging provides image loading, saving and manipulation.

[![Build Status](https://secure.travis-ci.org/kurocha/dream-imaging.svg)](http://travis-ci.org/kurocha/dream-imaging)

## Usage

`Dream::Imaging` provides a few useful classes for dealing with pixel data: `Dream::Imaging::PixelBuffer` which has an associated `Dream::Imaging::PixelBufferLayout`, and `Dream::Imaging::Image` represents a typical 2D image.

`Dream::Imaging::Image` can be loaded by `Dream::Resources::Loader`, but can also be loaded and saved directly:

```c++
auto input_data = ref(new Dream::Core::LocalFileData(path));
auto image = Image::load_from_data(input_data);
auto output_data = Image::save_to_data(image);
```

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## License

Released under the MIT license.

Copyright, 2013, by [Samuel G. D. Williams](http://www.codeotaku.com/samuel-williams).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.