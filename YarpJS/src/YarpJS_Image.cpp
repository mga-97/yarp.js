

#include <stdio.h>
#include <string>

#include "YarpJS_Image.h"

#include <highgui.h>





using namespace v8;

Nan::Persistent<v8::FunctionTemplate>  YarpJS_Image::constructor;



void YarpJS_Image::compress(int compression_quality)
{
    internalImage = cv::Mat((IplImage *) this->getYarpObj()->getIplImage());
    std::vector<int> p;
    std::string encodeString;
    if(compression_type == PNG)
    {
      p.push_back(CV_IMWRITE_PNG_COMPRESSION);
      p.push_back(0);
      encodeString = ".png";
    }
    else
    {
      p.push_back(CV_IMWRITE_JPEG_QUALITY);
      p.push_back(compression_quality);
      encodeString = ".jpg";
    }
        
    cv::imencode(encodeString,internalImage, internalBuffer, p);
}



NAN_METHOD(YarpJS_Image::Copy) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());
  
  YarpJS_Image* target = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info[0]->ToObject());
  obj->getYarpObj()->copy(*(target->getYarpObj()));

}


NAN_METHOD(YarpJS_Image::ToBinary) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());
  
  int compression_quality = info[0]->IntegerValue();

  if(info[0]->IsUndefined())
    obj->compress();
  else
    obj->compress(compression_quality);
  
  info.GetReturnValue().Set(Nan::CopyBuffer((char *) &obj->internalBuffer
    [0], obj->internalBuffer.size()*sizeof(unsigned char)).ToLocalChecked());


}


NAN_METHOD(YarpJS_Image::GetCompressionType) {

  YarpJS_Image* obj = Nan::ObjectWrap::Unwrap<YarpJS_Image>(info.This());
  
  obj->compress();

  std::string compression_type_string = (obj->compression_type == PNG) ? "png":"jpg";

  info.GetReturnValue().Set(Nan::New(compression_type_string.c_str()).ToLocalChecked());

}


NAN_METHOD(YarpJS_Image::GetObjType) {

  info.GetReturnValue().Set(Nan::New("image").ToLocalChecked());
}















