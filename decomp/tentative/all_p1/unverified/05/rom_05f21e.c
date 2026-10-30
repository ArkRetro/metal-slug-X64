/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05f21e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5f21e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  if (*(int *)((int)unaff_A6 + 0x62) == 0) {
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    *unaff_A6 = 0x5f25c;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar1 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_143dc();
    }
    else {
      candidate_1456a();
    }
    candidate_4c24();
    if (((((bool)uVar1) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
        ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
       ((*(char *)((int)unaff_A6 + 0x89) == '\0' && (*(char *)((int)unaff_A6 + 0x8d) == '\0')))) {
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
      unaff_A6[0x21] = unaff_A6[8];
      candidate_1b206();
      candidate_4256();
    }
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      unaff_A6[0x14] = 0x5eb62;
    }
    else {
      unaff_A6[0x14] = 0x5eb46;
    }
    bVar2 = false;
    candidate_427a();
    if ((bVar2) && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    if (((0x2f < *(short *)(unaff_A6 + 0xf)) && (*(short *)(unaff_A6 + 0xf) < 0x121)) &&
       (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      *unaff_A6 = candidate_5f0a8;
    }
    candidate_5fbee();
    candidate_5fd48();
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    return;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x5f39a;
  uVar1 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_4c24();
  if (((((bool)uVar1) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
      ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
     ((*(char *)((int)unaff_A6 + 0x89) == '\0' && (*(char *)((int)unaff_A6 + 0x8d) == '\0')))) {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
    unaff_A6[0x21] = unaff_A6[8];
    candidate_1b206();
    candidate_4256();
  }
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    unaff_A6[0x14] = 0x5eb62;
  }
  else {
    unaff_A6[0x14] = 0x5eb46;
  }
  bVar2 = false;
  candidate_427a();
  if (bVar2) {
    if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    bVar2 = true;
  }
  else {
    bVar2 = false;
  }
  if ((bVar2) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
    *unaff_A6 = candidate_5f0a8;
  }
  candidate_5fbee();
  candidate_5fd48();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
