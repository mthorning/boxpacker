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
  | AddContainer(Container.t)
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
  | AddContainer(newContainer) => {
      ...state,
      containers: Array.append(state.containers, [|newContainer|]),
    }
  | ToggleContainerSelection(selection) => {
      ...state,
      selectedContainer: toggleSelection(state.selectedContainer, selection),
    }
  | EditContainerName(id, name) => state

  | DeleteContainer(id) => {
      ...state,
      containers:
        Belt.Array.keep(state.containers, container => container.id != id),
    }
  };
};