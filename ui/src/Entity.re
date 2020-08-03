open Utils;
open Css;

module Styles = {
    let container = selected => {
        let base = [2->px->padding, 1->px->borderBottom(solid, black)];
        style(selected ? Theme.selectionColor @ base : base);
    };
    let entity =
        style([
              display(`flex),
              justifyContent(spaceBetween),
              alignItems(center),
        ]);
    let name =
        style([
              overflow(hidden),
              textOverflow(ellipsis),
        ]);

};

[@react.component]
let make =
(
    ~edit,
    ~selected,
    ~onNameEdit as onSubmit,
    ~name,
    ~id,
    ~displayBeforeClick,
    ~displayOnClick,
) => {

    <div className={Styles.container(selected)}>
    {edit
        ? <InputBox placeholder=name onSubmit={onSubmit(id)} />
            : <div className=Styles.entity>
            <span className=Styles.name>name->s</span>
            {selected ? displayOnClick(id) : displayBeforeClick()}
        </div>}
    </div>;
};
