/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05f0a8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5f0a8(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  if (*(int *)((int)unaff_A6 + 0x62) == 0) {
    uVar2 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1f) = uVar2;
    if (*(char *)(unaff_A6 + 0x22) == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    *unaff_A6 = 0x5f0f6;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar3 = 0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      candidate_143dc();
    }
    else {
      candidate_1456a();
    }
    candidate_4c24();
    if (((((bool)uVar3) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
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
    bVar4 = false;
    candidate_427a();
    if ((bVar4) && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    if ((*(char *)((int)unaff_A6 + 0x8d) == '\0') &&
       (sVar1 = *(short *)(unaff_A6 + 0x1f), *(short *)(unaff_A6 + 0x1f) = sVar1 + -1, sVar1 == 0))
    {
      *(undefined2 *)(unaff_A6 + 0x1f) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
      candidate_5fa5c();
      bVar4 = *(ushort *)((int)unaff_A6 + 0x76) < *(ushort *)(unaff_A6 + 0x1e);
      if (*(ushort *)((int)unaff_A6 + 0x76) == *(ushort *)(unaff_A6 + 0x1e)) {
        candidate_5fb06();
        if (bVar4) {
          *unaff_A6 = candidate_5f0a8;
        }
        else {
          *unaff_A6 = candidate_5ef5c;
        }
      }
      else {
        *unaff_A6 = candidate_5ef5c;
      }
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
  uVar3 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_4c24();
  if (((((bool)uVar3) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
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
  bVar4 = false;
  candidate_427a();
  if (bVar4) {
    if (*(char *)((int)unaff_A6 + 0x8d) != '\0') {
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    bVar4 = true;
  }
  else {
    bVar4 = false;
  }
  if ((bVar4) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
    *unaff_A6 = candidate_5f0a8;
  }
  candidate_5fbee();
  candidate_5fd48();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
