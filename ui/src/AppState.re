open Container;

type id = int;

type selection =
  | Nothing
  | Selected(id)
  | Editing(id);

type state = {
  containers: array(Container.t),
  selectedContainer: selection,
};

type action =
  | LoadContainers(array(Container.t))
  | AddContainer(string)
  | ToggleContainerSelection(selection)
  | EditContainerName(id, string)
  | DeleteContainer(id);

let toggleSelection =
    (selectedEntity: selection, selection: selection): selection => {
  switch (selectedEntity, selection) {
  | (Editing(selectedId), Editing(clickedId))
  | (Selected(selectedId), Selected(clickedId)) =>
    selectedId === clickedId ? Nothing : selection
  | _ => selection
  };
};

let reducer = (state, action) => {
  switch (action) {
  | LoadContainers(containers) => {...state, containers}
  | AddContainer(name) => state
  | ToggleContainerSelection(selection) => {
      ...state,
      selectedContainer: toggleSelection(state.selectedContainer, selection),
    }
  | EditContainerName(id, name) => state

  | DeleteContainer(id) => state
  };
};

module XHR = {
  type request;
  type response = string;
  [@bs.new] external makeXMLHttpRequest: unit => request = "XMLHttpRequest";
  [@bs.send]
  external addEventListener: (request, string, unit => unit) => unit =
    "addEventListener";
  [@bs.get] external response: request => response = "response";
  [@bs.send] external open_: (request, string, string) => unit = "open";
  [@bs.send] external send: request => unit = "send";
  [@bs.send] external sendWithData: (request, string) => unit = "send";
  [@bs.send] external abort: request => unit = "abort";

  type status('a) =
    | Loading
    | Error
    | Loaded('a);

  let useApiData = (~endpoint, ~decoder) => {
    let (data, setData) = React.useState(() => Loading);

    React.useEffect0(() => {
      let request = makeXMLHttpRequest();
      request->addEventListener("load", () => {
        setData(_ => Loaded(request->response->decoder))
      });
      request->addEventListener("error", () => {setData(_ => Error)});
      request->open_("GET", endpoint);
      request->send;
      Some(() => {request->abort});
    });
    (data, setData);
  };

  // This should be the setData returned from the hook. Too tired now.
  let postData = (~endpoint, ~decoder, ~data) => {
    let request = makeXMLHttpRequest();
    request->addEventListener("load", () => {
      Js.log(request->response->decoder)
    });
    request->addEventListener("error", () => {Js.log("Failed to post data")});
    request->open_("POST", endpoint);
    request->sendWithData(data);
    //Some(() => {request->abort});
  };
};