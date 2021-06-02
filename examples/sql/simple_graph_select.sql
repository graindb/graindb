// SELECT a.name
// FROM Person AS a, Knows AS k, Person AS b
// WHERE k.src_id=b._id AND k.dest_id=a.id AND b.name='Alice';
SELECT a.name FROM social AS Person:a<-[Knows:k]-Person:b WHERE b.name='Alice';

SELECT a.name FROM social AS ?:a<-[Knows|Friends:k]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends:k*2..4]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends*2..4]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends*..4]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends*2..]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends*2]-Person:b WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends*]-Person:b WHERE b.name='Alice';

SELECT a.name FROM social AS ?:a<-[Knows|Friends:k]-Person:b-[Knows]->Person:c WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends:k]-Person:b<-[Knows]-Person:c WHERE b.name='Alice';
SELECT a.name FROM social AS ?:a<-[Knows|Friends:k]-Person:b-[Classmate]-Person:c WHERE b.name='Alice';

SELECT a.name FROM social AS ?:a<-[Knows|Friends:k]-Person:b-[Classmate]-Person:c, class AS cl WHERE b.name='Alice';

SELECT p1.name, p2.name, k.since FROM social AS Company:c<-[Works:w]-Person:p1-[Knows:k]->Person:p2 WHERE p1.age>20;
SELECT p1.name, p2.name, k.since FROM social AS ?<-[Works:w]-Person:p1-[Knows:k]->Person:p2 WHERE p1.age>20;
SELECT p1.name, p2.name, k.since FROM social AS ?:c<-[Works:w]-Person:p1-[?:k]->Person:p2 WHERE p1.age>20;
SELECT p1.name, p2.name, k.since FROM social AS ?:c<-[Works:w]-Person:p1-[?:k]->Person:p2 WHERE p1.age>20;
