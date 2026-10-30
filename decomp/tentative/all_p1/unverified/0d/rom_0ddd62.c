/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddd62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddd62(void)

{
  undefined4 uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar2;

  candidate_de3c2();
  if ((bool)in_CF) {
    candidate_1b1dc();
    candidate_4256();
    *unaff_A6 = 0xdde62;
    uVar2 = 0;
    func_0x000143e4();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = 0xddcf4;
      uVar2 = 0;
    }
  }
  else {
    uVar1 = 0x40000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      uVar1 = 0xfffc0000;
    }
    *(undefined4 *)((int)unaff_A6 + 0x62) = uVar1;
    candidate_4256();
    *unaff_A6 = 0xddd96;
    uVar2 = 0;
    candidate_1456a();
    candidate_427a();
    candidate_de318();
    if ((bool)uVar2) {
      *unaff_A6 = 0xddcf4;
      uVar2 = 0;
    }
    candidate_de37e();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_ddf5a;
      uVar2 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_ddf5a;
      uVar2 = 0;
    }
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x76) + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  candidate_1aeda();
  if (!(bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0xff;
  }
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) &&
     (uVar2 = 0, *(char *)((int)unaff_A6 + 0x87) != '\0')) {
    *unaff_A6 = candidate_de064;
    uVar2 = 0;
  }
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_de06e;
  }
  return;
}
