let s = React.string;

let mapElements = (list, callback) => {
  list->Belt.List.map(callback)->Array.of_list->React.array;
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