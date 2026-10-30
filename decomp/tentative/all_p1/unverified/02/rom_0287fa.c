/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0287fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_287fa(void)

{
  byte bVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_1ad6e();
  uVar2 = 0;
  candidate_9ea8();
  candidate_1af9a();
  if ((!(bool)uVar2) &&
     (*(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe,
     *(ushort *)(unaff_A6 + 0xf) <= *(ushort *)(extraout_A0 + 0x3c))) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  bVar1 = *(byte *)(unaff_A6 + 0x1f);
  if (bVar1 == 2) {
    bVar1 = candidate_29c6();
    bVar1 = bVar1 & 1;
  }
  if (bVar1 == 0) {
    *(undefined1 *)(unaff_A6 + 0x1f) = 1;
    *(byte *)((int)unaff_A6 + 0x7d) = *(byte *)((int)unaff_A6 + 0x7d) & 0xef;
    candidate_4256();
    *unaff_A6 = 0x28930;
    uVar2 = 0;
    candidate_28e2c();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_28948;
      uVar2 = 0;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x1f) = 0;
    *(byte *)((int)unaff_A6 + 0x7d) = *(byte *)((int)unaff_A6 + 0x7d) | 0x10;
    candidate_4256();
    *unaff_A6 = 0x28886;
    uVar2 = 0;
    candidate_28e2c();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_2889e;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_289a6;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_409a();
  if (!(bool)uVar2) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
