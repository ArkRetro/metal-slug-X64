/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x083e2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_83e2c(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_4256();
  candidate_40bc();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
  *unaff_A6 = 0x83e5e;
  uVar4 = 0;
  candidate_1456a();
  candidate_427a();
  if ((bool)uVar4) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_83f28;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_83f28;
  }
  *(int *)((int)unaff_A6 + 0x62) =
       (*(int *)((int)unaff_A6 + 0x6e) - *(int *)((int)unaff_A6 + 0x62) >> 3) +
       *(int *)((int)unaff_A6 + 0x62);
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_8404e;
  }
  candidate_4c24();
  if (*(char *)((int)unaff_A6 + 0x5a) != '\0') {
    if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) &&
       (*(char *)((int)unaff_A6 + 0x5a) == '\x06')) {
      candidate_120f8();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_8404e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    *unaff_A6 = candidate_84010;
  }
  bVar5 = false;
  candidate_1945c();
  if (!bVar5) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
