open Types;

type id = int;

type selection =
  | Nothing
  | Selected(id)
  | Editing(id);

type state = {
  containers: array(Container.t),
  items: array(Item.t),
  selectedContainer: selection,
  selectedItem: selection,
};

type action =
  | LoadContainers(array(Container.t))
  | AddContainer(Container.t)
  | ToggleContainerSelection(selection)
  | EditContainerName(id, string)
  | DeleteContainer(id)
  | LoadItems(array(Item.t))
  | AddItem(Item.t)
  | ToggleItemSelection(selection)
  | EditItemName(id, string)
  | DeleteItem(id);

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
  | LoadItems(items) => {...state, items}
  | AddItem(newItem) => {
      ...state,
      items:
        state.items->Belt.Array.keep(item => item.id !== newItem.id)
        |> Array.append([|newItem|]),
    }
  | ToggleItemSelection(selection) => {
      ...state,
      selectedItem: toggleSelection(state.selectedItem, selection),
    }
  | EditItemName(id, name) => state

  | DeleteItem(id) => {
      ...state,
      items: Belt.Array.keep(state.items, item => item.id != id),
    }
  };
};