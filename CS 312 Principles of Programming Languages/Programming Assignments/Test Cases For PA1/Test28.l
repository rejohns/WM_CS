fun reverse with list =
if isNil #list then list else
(last list)@(reverse (sub list))
in
(reverse 1@2@3@4)