open AppState;
open Utils;

module Styles = {
  open Css;

  let container =
    style([
      5->px->borderRadius,
      300->px->width,
      5->px->padding,
      1->px->border(solid, black),
    ]);

  let inputContainer = style([10->px->padding]);

  let ul = style([0->px->padding]);

  let li = style([cursor(`pointer), listStyleType(none)]);
};

type delete =
  | Delete(id, string)
  | NoDelete;

[@react.component]
let make = (~state, ~dispatch) => {
  React.useEffect0(() => {
    let abort =
      Xhr.get(
        ~endpoint=Container.endpoint,
        ~onLoad=
          json => json->Container.array_decoder->LoadContainers->dispatch,
        ~onError=error => Js.log(error),
      );
    Some(abort);
  });

  let onClick = selection => dispatch(ToggleContainerSelection(selection));

  let clickHandler = id => Utils.DoubleClick.handler(onClick, id);

  let (showDelete, setShowDelete) = React.useState(_ => NoDelete);

  let confirmDeletion = (id, _) => {
    Xhr.delete(
      ~endpoint=Container.endpoint,
      ~onLoad=() => dispatch(DeleteContainer(id)),
      ~onError=error => Js.log(error),
      ~id,
    );
  };

  let onDeleteClick = (id, event) => {
    ReactEvent.Mouse.stopPropagation(event);
    setShowDelete(_ => Delete(id, "Are you sure you want to delete?"));
  };

  let onSubmit = (name, resetInput) => {
    let newContainer = Js.Dict.empty();
    Js.Dict.set(newContainer, "name", Js.Json.string(name));
    let _ =
      Xhr.post(
        ~endpoint=Container.endpoint,
        ~data=Js.Json.stringify(Js.Json.object_(newContainer)),
        ~onLoad=json => json->Container.decoder->AddContainer->dispatch,
        ~onError=error => Js.log(error),
      );
    resetInput();
    ();
  };

  let onEdit = (id, name, resetInput) => {
    dispatch(EditContainerName(id, name));
    resetInput();
  };

  <>
    {switch (showDelete) {
     | Delete(id, message) =>
       <DeleteModal
         closeModal={_ => setShowDelete(_ => NoDelete)}
         message
         onConfirmDeletion={confirmDeletion(id)}
       />
     | NoDelete => React.null
     }}
    <div className=Styles.container>
      <div className=Styles.inputContainer> <InputBox onSubmit /> </div>
      <ul className=Styles.ul>
        {state.containers
         ->mapElementArray(container => {
             let (selected, edit) =
               switch (state.selectedContainer) {
               | Selected(id) => (id === container.id, false)
               | Editing(id) => (false, id === container.id)
               | _ => (false, false)
               };
             <li
               onClick={_ => clickHandler(container.id)}
               key={string_of_int(container.id)}
               className=Styles.li>
               <Entity
                 name={container.name}
                 id={container.id}
                 onEdit
                 displayOnEntityClick={id =>
                   <DeleteIcon onClick={onDeleteClick(id)} />
                 }
                 edit
                 selected
               />
             </li>;
           })}
      </ul>
    </div>
  </>;
};