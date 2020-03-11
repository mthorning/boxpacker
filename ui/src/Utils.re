let s = React.string;

let mapElements = (list, callback) => {
  list->Belt.List.map(callback)->Array.of_list->React.array;
};

/* Old method */
let mapElementArray = (array, callback) => {
  array->Belt.Array.map(callback)->React.array;
};

let uuid = name => {
  let id =
    name ++ "-" ++ Js.Date.now()->int_of_float->Random.int->string_of_int;
  id;
};

/* Icons */
module DeleteIcon = {
  [@bs.module "react-icons/ai"] [@react.component]
  external make: (~onClick: ReactEvent.Mouse.t => unit) => React.element =
    "AiOutlineDelete";
};

module DoubleClick = {
  open AppState;

  type clickEvent =
    | NotClicked
    | Clicked(Js_global.timeoutId);

  let handler = clickHandler => {
    let clickState = ref(NotClicked);

    (id: id) => {
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
};