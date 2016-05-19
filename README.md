# SafeCallbacks [![Build Status](https://travis-ci.org/skyscribe/safeCallback.png)](https://travis-ci.org/skyscribe/safeCallback)
This is a demo code to verify the idea of how to manage asynchronous callbacks safely, when stored callback might be handled by an external class, while the owner itself might die during live execution of its wrapped memeber function (in the callback).

The cleaner way to get rid of this is 
- Let callback manager provides removal/unregister interface in its API
- Upon removal, callback manager shall check its internal status, and ensure
    - Any queued taks are removed completely
    - **Already scheduled tasks shall be finished** (if any) before return back
    - Asychronous control and synchronization is needed definitely
- The callback provider shall removal itself before die

Above idea wouldn't work if you get no touch over the callback manager codebase, like you're working with a third party library or so. The solution provided in this demo provides another way out - do synchronization from provider side between callback execution and class destruction:
- The provider check if callback is ongoing (possibly **asynchronous**) and blocks the desctructor execution
- Callback implementation also check if object is under destruction and synchronize exclusive access
