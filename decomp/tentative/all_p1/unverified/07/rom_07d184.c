/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07d184.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7d184(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0x7d190;
  uVar4 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (((*(char *)(unaff_A6 + 0x18) == -0x78) && (*(char *)(unaff_A6[3] + 0x61) == -1)) ||
     (*(char *)((int)unaff_A6 + 0x61) == -1)) {
    *unaff_A6 = candidate_7d20c;
  }
  if (*(char *)((int)unaff_A6 + 0x61) == -0x12) {
    *(undefined1 *)((int)unaff_A6 + 0x6f) = 0x1e;
    *unaff_A6 = candidate_7d048;
  }
  bVar3 = *(char *)(unaff_A6[3] + 0x61) != -1;
  if (!bVar3) {
    *unaff_A6 = candidate_7d20c;
  }
  candidate_7d8f0();
  if (bVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
