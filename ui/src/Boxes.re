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
    Container.fetch_all(containers => dispatch(LoadContainers(containers)));
    None;
  });

  let onClick = selection => dispatch(ToggleContainerSelection(selection));

  let clickHandler = id => Utils.DoubleClick.handler(onClick, id);

  let (showDelete, setShowDelete) = React.useState(_ => NoDelete);

  let onDeleteClick = (id, event) => {
    ReactEvent.Mouse.stopPropagation(event);
  };

  let onConfirmDeletion = (id, _) => {
    dispatch(DeleteContainer(id));
  };

  let onSubmit = (name, resetInput) => {
    dispatch(AddContainer(name));
    resetInput();
  };

  let onEdit = (id, name, resetInput) => {
    dispatch(EditContainerName(id, name));
    resetInput();
  };

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