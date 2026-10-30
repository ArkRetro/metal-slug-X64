/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08255c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8255c(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_4256();
  candidate_1b1dc();
  *unaff_A6 = 0x82578;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar4) {
    sVar3 = candidate_260c8();
    *(short *)(unaff_A6 + 0x1d) = sVar3;
    if (sVar3 < 0) {
      *unaff_A6 = candidate_82614;
    }
    else if (*(char *)((int)unaff_A6 + 0x7d) == '\0') {
      *unaff_A6 = candidate_825c2;
    }
    else {
      *unaff_A6 = candidate_82614;
    }
  }
  uVar4 = false;
  if (0 < *(short *)(unaff_A6 + 0x1d)) {
    sVar3 = *(short *)(unaff_A6 + 0x1d);
    uVar4 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x1d) = sVar3 + -1;
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_826c2;
    uVar4 = 0;
  }
  candidate_1945c();
  if (!(bool)uVar4) {
    return;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
