import lambda;

// The layer to draw on the screen
class CustomLayer extends lambda.Layer {
  // Handles when layer is attached to screen
  OnAttach(){}

  // Handles when layer is detached from screen
  OnDetach(){}

  // Called when engine finishes an update.
  OnUpdate(last_update_time) {}

  // Called when engine finishes a render
  OnRender() {
    lambda.Renderer.DrawString("Hello world!", GetWidth() / 2, GetHeight() / 2);
  }
}

// The application instance the engine will run
class MyApp extends lambda.Application {
  constructor(appName) {
    this.appName = appName;

    // Builtin funcction that adds your layer to your app
    PushLayer(CustomLayer());
  }
}

// Function that lets the engine grab your application instance to start running your app
func CreateApplication() {
  return MyApp();
}
