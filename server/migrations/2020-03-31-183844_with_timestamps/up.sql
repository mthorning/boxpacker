DELETE FROM containers;
ALTER TABLE containers ADD created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP;
ALTER TABLE containers ADD updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP;
SELECT diesel_manage_updated_at('containers');

DELETE FROM items;
ALTER TABLE items ADD created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP;
ALTER TABLE items ADD updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP;
SELECT diesel_manage_updated_at('items');
