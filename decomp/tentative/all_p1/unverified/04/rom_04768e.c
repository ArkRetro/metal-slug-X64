/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04768e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4768e(void)

{
  short sVar1;
  byte bVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = 0x4769c;
  candidate_49e9e();
  candidate_4936c();
  func_0x000143e4();
  candidate_49b20();
  candidate_427a();
  bVar2 = *(byte *)((int)unaff_A6 + 0x96);
  uVar3 = 0xfc < bVar2;
  *(byte *)((int)unaff_A6 + 0x96) = bVar2 + 3;
  candidate_1b04a();
  if (!(bool)uVar3) {
    if (*(char *)((int)unaff_A6 + 0x9b) == '\0') {
      if (*(short *)(unaff_A6 + 0x21) == 0) {
        if (unaff_A6[0xc] == 0) {
          *unaff_A6 = candidate_4771a;
        }
        uVar3 = 0;
      }
      else {
        bVar4 = false;
        candidate_49e74();
        if (!bVar4) {
          *unaff_A6 = 0x47608;
        }
        sVar1 = *(short *)(unaff_A6 + 0x21);
        uVar3 = sVar1 == 0;
        *(short *)(unaff_A6 + 0x21) = sVar1 + -1;
      }
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x9b) = 0;
      *unaff_A6 = 0x47608;
      uVar3 = 0;
    }
  }
  candidate_49ece();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_4783a;
  }
  candidate_4900c();
  return;
}
