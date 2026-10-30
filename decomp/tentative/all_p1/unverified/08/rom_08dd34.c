/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08dd34.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8dd34(void)

{
  char cVar1;
  char cVar2;
  undefined1 uVar3;
  undefined4 *unaff_A6;

  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar3;
    candidate_4256();
    *unaff_A6 = 0x8dd6e;
    uVar3 = 0;
    candidate_8f4ce();
    candidate_427a();
    candidate_1b284();
    if (!(bool)uVar3) {
      cVar1 = *(char *)((int)unaff_A6 + 0x8f);
      cVar2 = cVar1 + -1;
      *(char *)((int)unaff_A6 + 0x8f) = cVar2;
      if (cVar2 == '\0' || '\0' >= cVar1) {
        *unaff_A6 = candidate_8dede;
      }
      uVar3 = (cVar2 != '\0' && '\0' < cVar1) && cVar1 == '\0';
      candidate_8f752();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_8df42;
        uVar3 = 0;
      }
    }
  }
  else {
    candidate_1b1dc();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    uVar3 = candidate_260c8();
    *(undefined1 *)((int)unaff_A6 + 0x8f) = uVar3;
    candidate_4256();
    *unaff_A6 = 0x8ddd4;
    uVar3 = 0;
    candidate_8f4ce();
    candidate_427a();
    candidate_1b284();
    if ((!(bool)uVar3) && (candidate_8f752(), (bool)uVar3)) {
      *unaff_A6 = candidate_8df42;
      uVar3 = 0;
    }
  }
  candidate_8e15a();
  *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) & 0xbf;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar3 = 0;
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 0x40;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_8e0d4;
    uVar3 = 0;
  }
  candidate_8f432();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_8e128;
  }
  return;
}
