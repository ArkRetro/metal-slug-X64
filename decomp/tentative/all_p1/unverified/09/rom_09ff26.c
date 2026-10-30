/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ff26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ff26(void)

{
  byte *pbVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_1ad6e();
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  candidate_9ea8();
  if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x9ff92;
  uVar3 = 0;
  candidate_a0866();
  if ((*(byte *)(unaff_A6 + 0x24) & 8) != 0) {
    candidate_427a();
  }
  candidate_4c24();
  if ((bool)uVar3) {
    if ((*(byte *)(unaff_A6 + 0x24) & 8) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      if (((*(byte *)(unaff_A6 + 0x24) & 0x20) == 0) &&
         (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, *(char *)(unaff_A6 + 0x18) == '\0')) {
        if ((*(byte *)(unaff_A6 + 0x24) & 2) == 0) {
          candidate_4256();
        }
        else {
          candidate_4256();
        }
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        pbVar1 = (byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x90);
        *pbVar1 = *pbVar1 | 0x10;
      }
    }
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_9fd60;
  }
  return;
}
