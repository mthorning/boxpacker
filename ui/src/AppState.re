open Types;

type id = int;

type selection =
    | Nothing
    | Selected(id)
    | Editing(id)
    | Deleting(id);

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
    | DeleteItem(id)
    | EditItem(id, Item.t);

    let toggleSelection =
    (selectedEntity: selection, selection: selection): selection => {
        switch (selectedEntity, selection) {
            | (Editing(selectedId), Editing(clickedId))
            | (Selected(selectedId), Selected(clickedId)) =>
            selectedId === clickedId ? Nothing : selection
            | _ => selection
            };
    };

let rec reducer = (state, action) => {
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
        | DeleteItem(id) => {
            ...state,
            items: Belt.Array.keep(state.items, item => item.id != id),
        }
        | EditItem(originalId, responseItem) => {
            originalId == responseItem.id
                ? {
                    ...state,
                    items: state.items->Belt.Array.map(
                        item => item.id == responseItem.id
                        ? responseItem
                        : item
                    ),
                    selectedItem: Nothing,
                }
            : state
                ->reducer(DeleteItem(originalId))
                ->reducer(EditItem(responseItem.id, responseItem))
        }
    }
};
