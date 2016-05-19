#include <node.h>
#include "v8-util.h"
#include "piv_manager.h"
#include <ykpiv/ykpiv.h>
#include <stdio.h>

using namespace v8;

void ListReaders(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  struct response resp = list_readers();
  if (resp.success) {
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, resp.message.c_str()));
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void VerifyPin(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::Utf8Value pin_param(args[0]);
  const char* pin_pointer = *pin_param;

  struct response resp = verify_pin(pin_pointer);
  if (resp.success) {
    args.GetReturnValue().Set(true);
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void Reset(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  struct response resp = reset();
  if (resp.success) {
    args.GetReturnValue().Set(true);
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void ChangePuk(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::Utf8Value current_puk_param(args[0]);
  const char *current_puk = *current_puk_param;

  String::Utf8Value new_puk_param(args[1]);
  const char *new_puk = *new_puk_param;

  struct response resp = change_puk(current_puk, new_puk);
  if (resp.success) {
    args.GetReturnValue().Set(true);
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void ChangePin(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::Utf8Value current_pin_param(args[0]);
  const char *current_pin = *current_pin_param;

  String::Utf8Value new_pin_param(args[1]);
  const char *new_pin = *new_pin_param;

  struct response resp = change_pin(current_pin, new_pin);
  if (resp.success) {
    args.GetReturnValue().Set(true);
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void Version(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  struct response resp = version();
  if (resp.success) {
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, resp.message.c_str()));
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void SetManagementKey(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::Utf8Value current_key_param(args[0]);
  const char *current_key = *current_key_param;

  String::Utf8Value new_key_param(args[1]);
  const char *new_key = *new_key_param;

  struct response resp = set_management_key(current_key, new_key);
  if (resp.success) {
    args.GetReturnValue().Set(true);
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void GenerateKey(const FunctionCallbackInfo<Value>& args) {

  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  String::Utf8Value mgm_key_param(args[0]);
  const char *mgm_key = *mgm_key_param;

  String::Utf8Value slot_param(args[1]);
  const char *slot = *slot_param;

  String::Utf8Value algorithm_param(args[2]);
  const char *algorithm = *algorithm_param;
  const unsigned char *algorithm_type = reinterpret_cast<const unsigned char *> (algorithm);

  String::Utf8Value key_format_param(args[3]);
  const char *key_format = *key_format_param;

  struct response resp = generate_key(mgm_key, slot, *algorithm_type, key_format);
  if (resp.success) {
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, resp.message.c_str()));
  } else {
    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, resp.error_message.c_str())));
  }
}

void GetAvailableAlgorithms(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);
    Local<Context> context = isolate->GetCurrentContext();

    Local<Map> algorithmsMap = Map::New(isolate);
    algorithmsMap->Set(context, String::NewFromUtf8(isolate, "3DES"), Int32::New(isolate, YKPIV_ALGO_3DES));
    algorithmsMap->Set(context, String::NewFromUtf8(isolate, "RSA1024"), Int32::New(isolate, YKPIV_ALGO_RSA1024));
    algorithmsMap->Set(context, String::NewFromUtf8(isolate, "RSA2048"), Int32::New(isolate, YKPIV_ALGO_RSA2048));
    algorithmsMap->Set(context, String::NewFromUtf8(isolate, "ECP256"), Int32::New(isolate, YKPIV_ALGO_ECCP256));
    algorithmsMap->Set(context, String::NewFromUtf8(isolate, "ECP384"), Int32::New(isolate, YKPIV_ALGO_ECCP384));

    args.GetReturnValue().Set(algorithmsMap);
}

void ImportCertificate(const FunctionCallbackInfo<Value>& args) {}
void RequestCertificate(const FunctionCallbackInfo<Value>& args) {}
void Status(const FunctionCallbackInfo<Value>& args) {}
void ImportKey(const FunctionCallbackInfo<Value>& args) {}
void UnlockPin(const FunctionCallbackInfo<Value>& args) {}
void DeleteCertificate(const FunctionCallbackInfo<Value>& args) {}
void ReadCertificate(const FunctionCallbackInfo<Value>& args) {}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "listReaders", ListReaders);
  NODE_SET_METHOD(exports, "verifyPin", VerifyPin);
  NODE_SET_METHOD(exports, "reset", Reset);
  NODE_SET_METHOD(exports, "changePuk", ChangePuk);
  NODE_SET_METHOD(exports, "changePin", ChangePin);
  NODE_SET_METHOD(exports, "version", Version);
  NODE_SET_METHOD(exports, "setManagementKey", SetManagementKey);
  NODE_SET_METHOD(exports, "getAvailableAlgorithms", GetAvailableAlgorithms);
  NODE_SET_METHOD(exports, "generateKey", GenerateKey);

  NODE_SET_METHOD(exports, "importCertificate", ImportCertificate);
  NODE_SET_METHOD(exports, "requestCertificate", RequestCertificate);
  NODE_SET_METHOD(exports, "status", Status);
  NODE_SET_METHOD(exports, "importKey", ImportKey);
  NODE_SET_METHOD(exports, "unlockPin", UnlockPin);
  NODE_SET_METHOD(exports, "deleteCertificate", DeleteCertificate);
  NODE_SET_METHOD(exports, "readCertificate", ReadCertificate);
}

NODE_MODULE(addon, Init)
