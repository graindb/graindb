###LDBC Light

Notations:
[+]: query has been tested
[x]: query has been written, but not tested yet
[^]: query has not been written yet. it might be complicated, need second thoughts

[+] Q1a/b/c [IC-1] [QID:1/2/3]
Given a start `Person`, find `Persons` with a given first name that the start `Person` is connected to by 1/2/3 steps via
`Knows` relationships.
Return `Persons'` workplaces and places of study.
Params:

[+] Q2a [IC-2] [QID:4]
Given a start `Person`, find (the most recent) `Message` from all of that `Person's` 1-step friends.
Only consider `Messages` created before a given date.
Params:
Notes: 1) cluster message with creationdate; 2) join between message and p2 is the main bottleneck, need change selectivity on message

[+] Q3a/b [IC-3] [QID:5/6]
Given a start `Person`, find `Persons` that are their 1/2 steps friends (excluding the start `Person`) that have made
`Messages` in a given country within a given period.
Params:
Notes: range filter on message is the main bottleneck

[+] Q4a [IC-4] [QID:7]
Given a start `Person`, find `Tags` that are attached to `Posts` that were created by that `Person’s` friends.
Only include `Tags` that were attached to friends’ `Posts` created within a given time interval.
Params:

[+] Q5a [IC-5] [QID:8]
`Friends` of a given `Person`, who join forums after a given time. find messages of that forum.
Params:
Notes: DuckDB will timeout on this query at sf 10. change join order

[IC-8] We skip this query, in which there is no m-to-m joins

[+] Q6a [IC-9] [QID:9]
Given a start `Person`, find (the most recent) `Message` from all of that `Person's` 2-step friends.
Only consider `Messages` created before a given date.
Params:
Notes: 1) cluster message with creationdate; 2) join between message and p2 is the main bottleneck, need change selectivity on message

[+] Q7a/b [IC-11] [QID:10/11]
Given a start `Person`, find that `Person’s` 1/2-step friends (excluding start `Person`) who started working in some
`Company` in a given `Country`, before a given date (year).
Params:
Notes: range lookup in pc_workfrom costs too much, consider clustering over it.

[IC-12] We skip this query, in which there is no m-to-m joins
[IC-13] We skip this query, in which there is no m-to-m joins
[IC-14] We skip this query, in which there is no m-to-m joins

[+] Q8a [BI-2] [QID:12]
Find the `Tags` under a given `TagClass` that were used in `Messages` during in the 100-day period starting at date and compare it with the 100-day period that follows.

[+] Q9a [BI-3] [QID:13]
Given a `TagClass` and a `Country`, find all the `Forums` created in the given `Country`, containing at least one `Message` with `Tags` belonging directly to the given `TagClass`, and count the `Messages` by the `Person` who created it and by the `Forum` which contains them.
The location of a `Forum` is identified by the location of the `Forum’s` moderator.
Notes: only 2/7 joins are RJs.

[^] Q10a [BI-4] [QID:14]

[+] Q11a [BI-5] [QID:15]
Get each `Person` (person) who has liked a `Message` (message) with a given `Tag` (direct relation, not transitive).

[BI-6] because it exhibits similar join pattern to BI-5
[BI-7] We skip this query, it doesn't have choke points related to join performance
Find all Messages that have a given Tag. Find the related Tags attached to (direct) reply Comments of these Messages, but only of those reply Comments that do not have the given Tag.
Group the Tags by name, and get the count of replies in each group.

[+] Q12a [BI-8] [QID:16]
Given a Tag, find all Persons that are interested in the Tag and have written a Message (Post or Comment) with a creationDate after a given date and that has a given Tag
Notes: the join of t2, mt2 and m2 costs much (not selective)

[BI-9] We skip this query, in which there is no m-to-m joins

[+] Q13a [BI-10] [QID:17]
Given a Person (startPerson), find all other Persons (expertCandidatePerson) that live in a given Country and are connected to given Person through the knows relation.
For each of these expertCandidatePerson nodes, retrieve all of their Messages that contain at least one Tag belonging to a given TagClass (direct relation not transitive).
Notes: only 4/8 joins are rjs.

[+] Q14a [BI-11] [QID:18]
Note: triangles.

[BI-12] We skip this query, it doesn't have choke points related to join performance

[^] Q15a [BI-13] [QID:19]

[^] Q16a [BI-14] [QID:20]
Seems complicated with many sub joins

[+] Q17a [BI-15] [QID:21]
Given a person, find all persons that the given person knows and also replied their posts.
Only consider Messages that were created in a Forum that was created within the timeframe (inter- val) [startDate, endDate]. Note that for Comments, the containing Forum is that of the Post that the comment (transitively) replies to.
Note: join between m1 and m2 is the main bottleneck

[BI-16] We skip this query, it doesn't have choke points related to join performance

[^] Q18a [BI-17] [QID:22]

[+] Q19a [BI-18] [QID:23]
For a given Person (person1) and a Tag (tag), recommend new friends (person2) who
• do not yet know person1
• have many mutual friends with person1
• are interested in tag.

[BI-19] We skip this query, it doesn't have choke points related to join performance
[BI-20] We skip this query, it doesn't have choke points related to join performance

[+] Q20a [IC-7] [QID:24]
Given a start Person, find the most recent likes on any of start Person’s Messages from his/her friends.

[^] Q21a [IC-10] [QID:25]
