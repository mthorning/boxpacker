open AppState;
open Utils;

type delete =
  | Delete(uuid, string)
  | NoDelete;

let nameIsUnique = (entities, name) =>
  !
    entities->Belt.List.some(entity => {
      switch (entity.eType) {
      | Item(_) => false
      | Box => entity.name === name
      }
    });

let itemCount = (entities, id) =>
  entities
  ->Belt.List.keep(entity => {
      switch (entity.eType) {
      | Box => false
      | Item(box) => box === id
      }
    })
  ->Belt.List.length;

let deleteMessage = itemCount => {
  let plural = itemCount !== 1 ? " items" : " item";

  "Are you sure you want to deleted this box and its "
  ++ itemCount->string_of_int
  ++ plural
  ++ "?";
};

[@react.component]
let make = (~state, ~dispatch) => {
  React.useEffect0(() => {
    Js.Promise.(
      ignore(
        Axios.get("/api/containers")
        |> then_(resp => resolve(dispatch(LoadBoxes(resp##data))))
        |> catch(error => resolve(Js.log(error))),
      )
    );
    None;
  });

  let clickHandler = selection => dispatch(ToggleBoxSelection(selection));

  let (showDelete, setShowDelete) = React.useState(_ => NoDelete);

  let onDeleteClick = (id, event) => {
    ReactEvent.Mouse.stopPropagation(event);
    let itemCount = itemCount(state.entities, id);
    switch (itemCount) {
    | 0 => dispatch(DeleteBox(id))
    | _ => setShowDelete(_ => Delete(id, deleteMessage(itemCount)))
    };
  };

  let onConfirmDeletion = (id, _) => {
    dispatch(DeleteBox(id));
  };

  let onSubmit = (name, resetInput) =>
    if (state.entities->nameIsUnique(name)) {
      dispatch(AddBox(name));
      resetInput();
    };

  let onEdit = (id, name, resetInput) => {
    dispatch(EditBoxName(id, name));
    resetInput();
  };

  let boxes =
    state.entities
    ->Belt.List.keep(entity => {
        switch (entity.eType) {
        | Box => true
        | Item(_) => false
        }
      });

  <>
    {switch (showDelete) {
     | Delete(id, message) =>
       <DeleteBoxModal
         closeModal={_ => setShowDelete(_ => NoDelete)}
         message
         onConfirmDeletion={onConfirmDeletion(id)}
       />
     | NoDelete => React.null
     }}
    <EntityList
      onEdit
      onSubmit
      clickHandler
      entities=boxes
      displayOnEntityClick={id => <DeleteIcon onClick={onDeleteClick(id)} />}
      selectedEntity={state.selectedBox}
    />
  </>;
};