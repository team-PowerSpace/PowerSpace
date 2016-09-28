#pragma once

class IScript {
public:
    virtual ~IScript() {}
    virtual void OnClick() = 0;
    virtual void OnTimer() = 0;
};
