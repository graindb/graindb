#!/bin/bash

# exit upon error
set -e

PG_DATA_DIR=/home/g35jin/ldbc_dataset/sf30/social_network
GRAINDB_DATA_DIR=/home/g35jin/graindb/third_party/ldbc/data/sf30
# PG_DB_NAME=${PG_DB_NAME:-ldbcsf1}
# PG_USER=${PG_USER:-$USER}
PG_FORCE_REGENERATE=${PG_FORCE_REGENERATE:-no}
# PG_PORT=${PG_PORT:-5432}

# we regenerate PostgreSQL-specific CSV files for posts and comments, if either
#  - it doesn't exist
#  - the source CSV is newer
#  - we are forced to do so by environment variable PG_FORCE_REGENERATE=yes

if [ ! -f $PG_DATA_DIR/dynamic/post_0_0-postgres.csv -o $PG_DATA_DIR/dynamic/post_0_0.csv -nt $PG_DATA_DIR/dynamic/post_0_0-postgres.csv -o "${PG_FORCE_REGENERATE}x" = "yesx" ]; then
  cat $PG_DATA_DIR/dynamic/post_0_0.csv | \
    awk -F '|' '{ print $1"|"$2"|"$3"|"$4"|"$5"|"$6"|"$7"|"$8"|"$9"|"$11"|"$10"|"}' > \
    $PG_DATA_DIR/dynamic/post_0_0-postgres.csv
fi
if [ ! -f $PG_DATA_DIR/dynamic/comment_0_0-postgres.csv -o $PG_DATA_DIR/dynamic/comment_0_0.csv -nt $PG_DATA_DIR/dynamic/comment_0_0-postgres.csv -o "${PG_FORCE_REGENERATE}x" = "yesx" ]; then
  cat $PG_DATA_DIR/dynamic/comment_0_0.csv | \
    awk -F '|' '{print $1"||"$2"|"$3"|"$4"||"$5"|"$6"|"$7"|"$8"||"$9 $10}' > \
    $PG_DATA_DIR/dynamic/comment_0_0-postgres.csv
fi

if [ ! -f $PG_DATA_DIR/dynamic/message_0_0.csv ]; then
  tail -n +1 $PG_DATA_DIR/dynamic/post_0_0-postgres.csv >> $PG_DATA_DIR/dynamic/message_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/comment_0_0-postgres.csv >> $PG_DATA_DIR/dynamic/message_0_0.csv
else
  rm $PG_DATA_DIR/dynamic/message_0_0.csv
  tail -n +1 $PG_DATA_DIR/dynamic/post_0_0-postgres.csv >> $PG_DATA_DIR/dynamic/message_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/comment_0_0-postgres.csv >> $PG_DATA_DIR/dynamic/message_0_0.csv
fi

if [ ! -f $PG_DATA_DIR/dynamic/likes_0_0.csv ]; then
  tail -n +1 $PG_DATA_DIR/dynamic/person_likes_comment_0_0.csv >> $PG_DATA_DIR/dynamic/likes_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/person_likes_post_0_0.csv >> $PG_DATA_DIR/dynamic/likes_0_0.csv
else
  rm $PG_DATA_DIR/dynamic/likes_0_0.csv
  tail -n +1 $PG_DATA_DIR/dynamic/person_likes_comment_0_0.csv >> $PG_DATA_DIR/dynamic/likes_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/person_likes_post_0_0.csv >> $PG_DATA_DIR/dynamic/likes_0_0.csv
fi

if [ ! -f $PG_DATA_DIR/dynamic/message_tag_0_0.csv ]; then
  tail -n +1 $PG_DATA_DIR/dynamic/comment_hasTag_tag_0_0.csv >> $PG_DATA_DIR/dynamic/message_tag_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/forum_hasTag_tag_0_0.csv >> $PG_DATA_DIR/dynamic/message_tag_0_0.csv
else
  rm $PG_DATA_DIR/dynamic/message_tag_0_0.csv
  tail -n +1 $PG_DATA_DIR/dynamic/comment_hasTag_tag_0_0.csv >> $PG_DATA_DIR/dynamic/message_tag_0_0.csv
  tail -n +2 $PG_DATA_DIR/dynamic/forum_hasTag_tag_0_0.csv >> $PG_DATA_DIR/dynamic/message_tag_0_0.csv
fi

cp $PG_DATA_DIR/dynamic/message_0_0.csv $GRAINDB_DATA_DIR/message.csv
cp $PG_DATA_DIR/dynamic/comment_0_0.csv $GRAINDB_DATA_DIR/comment.csv
cp $PG_DATA_DIR/dynamic/post_0_0.csv $GRAINDB_DATA_DIR/post.csv
cp $PG_DATA_DIR/dynamic/forum_0_0.csv $GRAINDB_DATA_DIR/forum.csv
cp $PG_DATA_DIR/dynamic/forum_hasMember_person_0_0.csv $GRAINDB_DATA_DIR/forum_person.csv
cp $PG_DATA_DIR/dynamic/forum_hasTag_tag_0_0.csv $GRAINDB_DATA_DIR/forum_tag.csv
cp $PG_DATA_DIR/static/organisation_0_0.csv $GRAINDB_DATA_DIR/organisation.csv
cp $PG_DATA_DIR/dynamic/person_0_0.csv $GRAINDB_DATA_DIR/person.csv
cp $PG_DATA_DIR/dynamic/person_email_emailaddress_0_0.csv $GRAINDB_DATA_DIR/person_email.csv
cp $PG_DATA_DIR/dynamic/person_hasInterest_tag_0_0.csv $GRAINDB_DATA_DIR/person_tag.csv
cp $PG_DATA_DIR/dynamic/person_knows_person_0_0.csv $GRAINDB_DATA_DIR/knows.csv
cp $PG_DATA_DIR/dynamic/likes_0_0.csv $GRAINDB_DATA_DIR/likes.csv
cp $PG_DATA_DIR/dynamic/person_likes_comment_0_0.csv $GRAINDB_DATA_DIR/clikes.csv
cp $PG_DATA_DIR/dynamic/person_likes_post_0_0.csv $GRAINDB_DATA_DIR/pslikes.csv
cp $PG_DATA_DIR/dynamic/person_speaks_language_0_0.csv $GRAINDB_DATA_DIR/person_language.csv
cp $PG_DATA_DIR/dynamic/person_studyAt_organisation_0_0.csv $GRAINDB_DATA_DIR/person_university.csv
cp $PG_DATA_DIR/dynamic/person_workAt_organisation_0_0.csv $GRAINDB_DATA_DIR/person_company.csv
cp $PG_DATA_DIR/static/place_0_0.csv $GRAINDB_DATA_DIR/place.csv
cp $PG_DATA_DIR/dynamic/message_tag_0_0.csv $GRAINDB_DATA_DIR/message_tag.csv
cp $PG_DATA_DIR/dynamic/comment_hasTag_tag_0_0.csv $GRAINDB_DATA_DIR/comment_tag.csv
cp $PG_DATA_DIR/dynamic/post_hasTag_tag_0_0.csv $GRAINDB_DATA_DIR/post_tag.csv
cp $PG_DATA_DIR/static/tagclass_0_0.csv $GRAINDB_DATA_DIR/tagclass.csv
cp $PG_DATA_DIR/static/tag_0_0.csv $GRAINDB_DATA_DIR/tag.csv


# dropdb --if-exists $PG_DB_NAME -U $PG_USER -p $PG_PORT
# createdb $PG_DB_NAME -U $PG_USER -p $PG_PORT --template template0 -l "C"
# psql -d $PG_DB_NAME -U $PG_USER -p $PG_PORT -a -f schema.sql -v "ON_ERROR_STOP=1"
# (cat snb-load.sql | sed "s|PATHVAR|$PG_DATA_DIR|g") | psql -d $PG_DB_NAME -U $PG_USER -p $PG_PORT -v "ON_ERROR_STOP=1"
# psql -d $PG_DB_NAME -U $PG_USER -p $PG_PORT -a -f schema_constraints.sql -v "ON_ERROR_STOP=1"
# psql -d $PG_DB_NAME -U $PG_USER -p $PG_PORT -a -f schema_foreign_keys.sql -v "ON_ERROR_STOP=1"
