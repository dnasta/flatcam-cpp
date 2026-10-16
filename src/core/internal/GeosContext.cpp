#include "GeosContext.h"

namespace flatcam::detail {

namespace {

void noticeHandler(const char*, void*) {}

void errorHandler(const char* message, void*) {
    GeosContext::lastError() = message ? message : "error desconocido de GEOS";
}

/// Crea y destruye el contexto GEOS de cada hilo.
struct Holder {
    GEOSContextHandle_t handle;
    Holder() : handle(GEOS_init_r()) {
        GEOSContext_setNoticeMessageHandler_r(handle, noticeHandler, nullptr);
        GEOSContext_setErrorMessageHandler_r(handle, errorHandler, nullptr);
    }
    ~Holder() { GEOS_finish_r(handle); }
    Holder(const Holder&) = delete;
    Holder& operator=(const Holder&) = delete;
};

}  // namespace

GEOSContextHandle_t GeosContext::handle() {
    thread_local Holder holder;
    return holder.handle;
}

std::string& GeosContext::lastError() {
    thread_local std::string message;
    return message;
}

}  // namespace flatcam::detail
