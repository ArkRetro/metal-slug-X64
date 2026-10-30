/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b39e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2b39e(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  *(undefined1 *)((int)unaff_A6 + 0x8e) = 1;
  *(undefined2 *)(unaff_A6 + 0x21) = 8;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_9ea8();
  candidate_1ad6e();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 1;
  *unaff_A6 = 0x2b3f2;
  uVar1 = candidate_2be98();
  if (uVar1 == 8) {
    *(undefined1 *)(unaff_A6 + 0x23) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 2;
    candidate_4256();
    *unaff_A6 = candidate_2b498;
    uVar1 = candidate_2be98();
    uVar2 = uVar1 < 9;
    if (uVar1 == 9) goto LAB_0002b5dc;
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2b4b8;
      uVar2 = 0;
    }
  }
  else {
    uVar2 = uVar1 < 9;
    if (uVar1 == 9) {
LAB_0002b5dc:
      candidate_2be98();
      bVar3 = false;
      candidate_40bc();
      if (bVar3) {
        candidate_1b220();
      }
      *unaff_A6 = candidate_2b138;
      return;
    }
    candidate_427a();
    candidate_2bfaa();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2b41e;
      uVar2 = 0;
    }
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
