type request;
type response = string;
[@bs.new] external makeXMLHttpRequest: unit => request = "XMLHttpRequest";
[@bs.get] external response: request => response = "response";
[@bs.send]
external addEventListener: (request, string, unit => unit) => unit =
  "addEventListener";
[@bs.send] external open_: (request, string, string) => unit = "open";
[@bs.send] external send: request => unit = "send";
[@bs.send] external sendWithData: (request, string) => unit = "send";
[@bs.send]
external setRequestHeader: (request, string, string) => unit =
  "setRequestHeader";
[@bs.send] external abort: request => unit = "abort";

let get = (~endpoint, ~onLoad, ~onError) => {
  let request = makeXMLHttpRequest();
  request->addEventListener("load", () => {onLoad(request->response)});
  request->addEventListener("error", () =>
    onError("There was an error fetching")
  );
  request->open_("GET", endpoint);
  request->send;

  () => {
    request->abort;
  };
};

let post = (~endpoint, ~onLoad, ~onError, ~data) => {
  let request = makeXMLHttpRequest();
  request->addEventListener("load", () => {onLoad(request->response)});
  request->addEventListener("error", () => {onError("Failed to post data")});
  request->open_("POST", endpoint);
  request->setRequestHeader("Content-Type", "application/json");
  request->sendWithData(data);

  () => {
    request->abort;
  };
};

let delete = (~endpoint, ~onLoad, ~onError, ~id) => {
  let request = makeXMLHttpRequest();
  request->addEventListener("load", onLoad);
  request->addEventListener("error", () => {
    onError("Failed to delete data")
  });
  request->open_("DELETE", endpoint ++ "/" ++ string_of_int(id));
  request->send;

  () => {
    request->abort;
  };
};