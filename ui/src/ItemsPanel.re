open AppState;
open Types;
open Utils;

module Styles = {
    open Css;

    let wrapper =
        style([
              5->px->borderRadius,
              300->px->width,
              5->px->padding,
              1->px->border(solid, black),
        ]);

    let inputItem = style([10->px->padding]);

    let ul = style([0->px->padding]);

    let li = style([cursor(`pointer), listStyleType(none)]);
};

    type delete =
| Delete(id, string)
| NoDelete;

let useDeleteComponents = (~dispatch) => {
    let (showDelete, setShowDelete) = React.useState(_ => NoDelete);
    let onDeleteClick = (id, event) => {
        ReactEvent.Mouse.stopPropagation(event);
        setShowDelete(_ => Delete(id, "Are you sure you want to delete?"));
    };

    let confirmDeletion = (id, _) => {
        Xhr.delete(
            ~endpoint=Item.endpoint,
            ~onLoad=() => dispatch(DeleteItem(id)),
            ~onError=error => Js.log(error),
            ~id,
        );
    };

    let button = id => <DeleteIcon onClick={onDeleteClick(id)} />;

    let modal =
        fun
            | Delete(id, message) =>
            <DeleteModal
            closeModal={_ => setShowDelete(_ => NoDelete)}
    message
        onConfirmDeletion={confirmDeletion(id)}
    />
        | NoDelete => React.null;

        (button, modal(showDelete));
};

[@react.component]
let make = (~state, ~dispatch) => {
    let (inputValue, setInputValue) = React.useState(_ => "");
    let valueCatcher = value => {
        setInputValue(_ => value);
        value;
    };
    let (deleteButton, deleteModal) = useDeleteComponents(~dispatch);

    React.useEffect0(() => {
        let abort =
            Xhr.get(
                ~endpoint=Item.endpoint,
                ~onLoad=json => json->Item.array_decoder->LoadItems->dispatch,
                ~onError=error => Js.log(error),
            );
        Some(abort);
    });

    let onClick = id =>
        Utils.DoubleClick.handler(
            selection => dispatch(ToggleItemSelection(selection)),
            id,
        );

    let onEdit = (id: AppState.id, name, _resetInput) => {
        let updateItem = Js.Dict.empty();
        Js.Dict.set(updateItem, "id", Js.Json.number(float_of_int(id)));
        Js.Dict.set(updateItem, "name", Js.Json.string(name));
        let _ = Xhr.patch(
            ~endpoint=Item.endpoint,
            ~onError=error => Js.log(error),
            ~onLoad=json => {
                let responseItem = json->Item.decoder;
                dispatch(EditItem(id, responseItem));
            },
            ~data=Js.Json.stringify(Js.Json.object_(updateItem)),
            ~id=id,
        );
    };

    let onSubmit = (name, resetInput) => {
        let newItem = Js.Dict.empty();
        Js.Dict.set(newItem, "name", Js.Json.string(name));
        Js.Dict.set(newItem, "total", Js.Json.number(1.0));
        let _ =
            Xhr.post(
                ~endpoint=Item.endpoint,
                ~data=Js.Json.stringify(Js.Json.object_(newItem)),
                ~onLoad=json => {
                    json->Item.decoder->AddItem->dispatch;
                    resetInput();
                },
                ~onError=error => Js.log(error),
            );
        ();
    };

    <>
        deleteModal
        <div className=Styles.wrapper>
        <div className=Styles.inputItem>
        <InputBox onSubmit valueCatcher />
        </div>
        <ul className=Styles.ul>
        {state.items
            ->Belt.Array.keep(item => Js.String.includes(inputValue, item.name))
                ->mapElementArray(item => {
                    let (selected, edit) =
                        switch (state.selectedItem) {
                            | Selected(id) => (id === item.id, false)
                            | Editing(id) => (false, id === item.id)
                            | _ => (false, false)
                            };
                    <li
                        onClick={_ => onClick(item.id)}
                    key={string_of_int(item.id)}
                    className=Styles.li>
                        <Entity
                        name={item.name}
                    id={item.id}
                    total={item.total}
                    onEdit
                        displayOnEntityClick=deleteButton
                        edit
                        selected
                        />
                        </li>;
                })}
    </ul>
        </div>
        </>;
};
