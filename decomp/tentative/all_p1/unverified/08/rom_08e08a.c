/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08e08a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8e08a(void)

{
  char cVar1;
  char cVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    bVar3 = false;
    candidate_8f4a0();
    if (bVar3) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x8df16;
    uVar4 = false;
    candidate_8f51c();
    if ((bool)uVar4) {
      *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) ^ 0x80;
      *unaff_A6 = candidate_8de84;
      uVar4 = 0;
    }
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_8ddfa;
      uVar4 = 0;
    }
    candidate_8f3b6();
  }
  else if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    uVar4 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar4;
    candidate_4256();
    *unaff_A6 = 0x8dd6e;
    uVar4 = 0;
    candidate_8f4ce();
    candidate_427a();
    candidate_1b284();
    if (!(bool)uVar4) {
      cVar1 = *(char *)((int)unaff_A6 + 0x8f);
      cVar2 = cVar1 + -1;
      *(char *)((int)unaff_A6 + 0x8f) = cVar2;
      if (cVar2 == '\0' || '\0' >= cVar1) {
        *unaff_A6 = candidate_8dede;
      }
      uVar4 = (cVar2 != '\0' && '\0' < cVar1) && cVar1 == '\0';
      candidate_8f752();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_8df42;
        uVar4 = 0;
      }
    }
  }
  else {
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    uVar4 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar4;
    candidate_4256();
    *unaff_A6 = 0x8ddd4;
    uVar4 = 0;
    candidate_8f4ce();
    candidate_427a();
    candidate_1b284();
    if ((!(bool)uVar4) && (candidate_8f752(), (bool)uVar4)) {
      *unaff_A6 = candidate_8df42;
      uVar4 = 0;
    }
  }
  candidate_8e15a();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar4 = 0;
  }
  candidate_8f432();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
