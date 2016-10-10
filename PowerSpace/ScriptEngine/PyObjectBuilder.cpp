#include <stdafx.h>
#include "PyObjectBuilder.h"
#include "CDrawableBuilder.h"

CPyObjectBuilder::CPyObjectBuilder(std::shared_ptr<IDrawable> object) {
	assert(CDrawable::DrawableType::count == 1);
	switch (object->GetType()) {
	case CDrawable::DrawableType::CDrawable: {
		CDrawableBuilder cdb(object);
		pObject = cdb.GetpObject();
		break;
	}
	default: {
		pObject = nullptr;
		break;
	}
	}
}

std::shared_ptr<PyObject> CPyObjectBuilder::GetpObject() {
	return pObject;
}