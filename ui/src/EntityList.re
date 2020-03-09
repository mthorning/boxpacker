open Utils;
open AppState;

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

type clickEvent =
  | NotClicked
  | Clicked(Js_global.timeoutId);

let doubleClickHandler = clickHandler => {
  let clickState = ref(NotClicked);

  (id: uuid) => {
    switch (clickState^) {
    | NotClicked =>
      let timeoutId =
        Js_global.setTimeout(
          () => {
            clickState := NotClicked;
            clickHandler(Selected(id));
            ();
          },
          200,
        );
      clickState := Clicked(timeoutId);

    | Clicked(timeoutId) =>
      Js_global.clearTimeout(timeoutId);
      clickState := NotClicked;
      clickHandler(Editing(id));
      ();
    };
  };
};

[@react.component]
let make =
    (
      ~onEdit,
      ~onSubmit,
      ~clickHandler,
      ~entities,
      ~displayOnEntityClick,
      ~selectedEntity,
    ) => {
  let onClick = doubleClickHandler(clickHandler);
  <div className=Styles.container>
    <div className=Styles.inputContainer> <InputBox onSubmit /> </div>
    <ul className=Styles.ul>
      {entities->mapElements(entity => {
         let (selected, edit) =
           switch (selectedEntity) {
           | Selected(id) => (id === entity.id, false)
           | Editing(id) => (false, id === entity.id)
           | _ => (false, false)
           };
         <li
           onClick={_ => onClick(entity.id)}
           key={entity.id}
           className=Styles.li>
           <Entity
             name={entity.name}
             id={entity.id}
             onEdit
             displayOnEntityClick
             edit
             selected
           />
         </li>;
       })}
    </ul>
  </div>;
};