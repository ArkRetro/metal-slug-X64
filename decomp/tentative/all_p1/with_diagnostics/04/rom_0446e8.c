/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0446e8.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint candidate_446e8(void)

{
  short sVar1;
  short sVar2;
  byte bVar3;
  uint uVar4;
  uint in_D1;
  uint unaff_D2;
  uint unaff_D3;
  uint unaff_D4;
  uint unaff_D5;
  short *psVar5;
  int unaff_A6;
  bool bVar6;
  bool bVar7;
  undefined8 uVar8;
  ushort uStack_a;
  short *psStack_8;
  short *psStack_4;

  psVar5 = (short *)(unaff_A6 + 0x86);
  bVar3 = *(byte *)(unaff_A6 + 0x81);
  uVar4 = (uint)bVar3;
  bVar6 = (char)bVar3 < '\0';
  bVar7 = bVar3 == 0;
  switch(bVar3) {
  case 0:
    return uVar4;
  case 1:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 2:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 3:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 4:
  case 0x12:
    register0x0000003c = (BADSPACEBASE *)&psStack_4;
    psStack_4 = psVar5;
code_r0x00044744:
    uVar4 = (uint)*(ushort *)(unaff_A6 + 0x7a);
code_r0x00044748:
    uVar4 = (uint)(ushort)((short)uVar4 - *psVar5);
code_r0x0004474a:
    in_D1 = (uint)*(ushort *)(unaff_A6 + 0x7c);
code_r0x0004474e:
    in_D1 = (uint)(ushort)((short)in_D1 - *(short *)(unaff_A6 + 0x8a));
code_r0x00044752:
    *(short *)((int)register0x0000003c + -4) = (short)in_D1;
    register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -8);
    *(short *)register0x0000003c = (short)uVar4;
code_r0x00044756:
    *(undefined4 *)((int)register0x0000003c + -4) = 0x4475c;
    candidate_2eb4();
    break;
  case 5:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 6:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 7:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 8:
    goto LAB_000447b4;
  case 9:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 10:
    uRam000447b2 = uRam000447b2 | 0x6000;
    return uVar4;
  case 0xb:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0xc:
LAB_000447b4:
    sVar2 = *(short *)(unaff_A6 + 0x7a);
    sVar1 = *psVar5;
    psStack_8 = (short *)CONCAT22(*(short *)(unaff_A6 + 0x7c) - *(short *)(unaff_A6 + 0x8a),
                                  psStack_8._2_2_);
    psStack_4 = psVar5;
    candidate_2eb4();
    uVar8 = candidate_2e7c();
    uVar4 = (uint)((ulonglong)uVar8 >> 0x20);
    in_D1 = (uint)uVar8;
    unaff_D3 = (uint)uStack_a;
    unaff_D2 = (uint)(ushort)(sVar2 - sVar1);
    psVar5 = psStack_8;
    goto LAB_00044772;
  case 0xd:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0xe:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0xf:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x10:
    return uVar4;
  case 0x11:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x13:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x14:
    goto code_r0x00044744;
  case 0x15:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x16:
    uRam00047976 = uRam00047976 | 0x9051;
  case 0x1c:
code_r0x00044750:
code_r0x00044754:
    goto code_r0x00044756;
  case 0x17:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x18:
    goto code_r0x00044748;
  case 0x19:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x1a:
    goto code_r0x0004474a;
  case 0x1b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x1d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x1e:
    goto code_r0x0004474e;
  case 0x1f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x20:
    goto code_r0x00044750;
  case 0x21:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x22:
    goto code_r0x00044752;
  case 0x23:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x24:
    goto code_r0x00044754;
  case 0x25:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x26:
    goto code_r0x00044756;
  case 0x27:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x28:
    break;
  case 0x29:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x2a:
    goto code_r0x0004475e;
  case 0x2b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x2c:
    break;
  case 0x2d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x2e:
    goto code_r0x0004475e;
  case 0x2f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x30:
    goto code_r0x00044760;
  case 0x31:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x32:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x33:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x34:
    goto code_r0x00044764;
  case 0x35:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x36:
    goto code_r0x00044766;
  case 0x37:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x38:
    uVar4 = (uint)(bVar3 | 0x7c);
    goto code_r0x0004476c;
  case 0x39:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x3a:
    register0x0000003c = (BADSPACEBASE *)0x4c9f000c;
    goto code_r0x00044770;
  case 0x3b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x3c:
    goto code_r0x0004476c;
  case 0x3d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x3e:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x3f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x40:
    goto code_r0x00044770;
  case 0x41:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x42:
    goto LAB_00044772;
  case 0x43:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x44:
    goto code_r0x00044774;
  case 0x45:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x46:
    goto code_r0x00044776;
  case 0x47:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x48:
    goto code_r0x00044778;
  case 0x49:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x4a:
    goto code_r0x0004477a;
  case 0x4b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x4c:
    goto code_r0x0004477c;
  case 0x4d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x4e:
    goto code_r0x0004477e;
  case 0x4f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x50:
    goto code_r0x00044780;
  case 0x51:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x52:
    goto code_r0x00044782;
  case 0x53:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x54:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x55:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x56:
    goto code_r0x00044786;
  case 0x57:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x58:
    goto code_r0x00044788;
  case 0x59:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x5a:
    candidate_3fb10 = candidate_3fb10 | 0x9a83;
    bVar6 = (int)((uint)candidate_3fb10 << 0x10) < 0;
    goto code_r0x00044790;
  case 0x5b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x5c:
    goto LAB_0004478c;
  case 0x5d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x5e:
    goto code_r0x0004478e;
  case 0x5f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x60:
    goto code_r0x00044790;
  case 0x61:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x62:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 99:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 100:
    goto code_r0x00044794;
  case 0x65:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x66:
    goto code_r0x00044796;
  case 0x67:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x68:
    goto LAB_0004479c;
  case 0x69:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x6a:
    halt_baddata();
  case 0x6b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x6c:
    goto LAB_0004479c;
  case 0x6d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x6e:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x6f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x70:
    goto code_r0x000447a0;
  case 0x71:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x72:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x73:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x74:
    goto code_r0x000447a4;
  case 0x75:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x76:
    goto code_r0x000447a6;
  case 0x77:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x78:
    goto LAB_000447ac;
  case 0x79:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x7a:
    goto code_r0x000447aa;
  case 0x7b:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x7c:
    goto LAB_000447ac;
  case 0x7d:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  case 0x7e:
    return (uint)(bVar3 | 0x81);
  case 0x7f:
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  default:
    if (*(char *)(unaff_A6 + 0x82) == '\0') {
      *(byte *)(unaff_A6 + 0x81) = *(byte *)(unaff_A6 + 0x81) & 0x7f;
      *(undefined1 *)(unaff_A6 + 0x82) = 0;
    }
    else {
      *(char *)(unaff_A6 + 0x82) = *(char *)(unaff_A6 + 0x82) + -1;
    }
    *(undefined4 *)(unaff_A6 + 0x8e) = 0;
    *(undefined4 *)(unaff_A6 + 0x92) = 0;
    return uVar4;
  }
code_r0x0004475e:
code_r0x00044760:
code_r0x00044764:
code_r0x00044766:
  *(undefined4 *)((int)register0x0000003c + -4) = 0x4476c;
  uVar8 = candidate_2e7c();
  uVar4 = (uint)((ulonglong)uVar8 >> 0x20);
  in_D1 = (uint)uVar8;
code_r0x0004476c:
  unaff_D2 = (uint)*(short *)register0x0000003c;
  unaff_D3 = (uint)*(short *)((int)register0x0000003c + 2);
  register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + 4);
code_r0x00044770:
  psVar5 = *(short **)register0x0000003c;
LAB_00044772:
  unaff_D4 = uVar4;
code_r0x00044774:
  unaff_D5 = in_D1;
code_r0x00044776:
  unaff_D2 = unaff_D2 << 0x10;
code_r0x00044778:
  unaff_D3 = unaff_D3 << 0x10;
code_r0x0004477a:
  unaff_D2 = unaff_D2 & 0xffff0000;
code_r0x0004477c:
  unaff_D3 = unaff_D3 & 0xffff0000;
code_r0x0004477e:
  unaff_D4 = unaff_D4 - unaff_D2;
code_r0x00044780:
  bVar6 = (int)(uVar4 ^ unaff_D4) < 0;
code_r0x00044782:
  if (!bVar6) {
code_r0x00044786:
    uVar4 = 0;
code_r0x00044788:
    *(undefined4 *)psVar5 = *(undefined4 *)(unaff_A6 + 0x7a);
  }
LAB_0004478c:
  unaff_D5 = unaff_D5 - unaff_D3;
code_r0x0004478e:
  bVar6 = (int)(in_D1 ^ unaff_D5) < 0;
code_r0x00044790:
  if (!bVar6) {
code_r0x00044794:
    in_D1 = 0;
code_r0x00044796:
    *(undefined4 *)(psVar5 + 2) = *(undefined4 *)(unaff_A6 + 0x7c);
  }
LAB_0004479c:
  *(uint *)(psVar5 + 4) = uVar4;
code_r0x000447a0:
  *(uint *)(psVar5 + 6) = in_D1;
code_r0x000447a4:
  uVar4 = in_D1 | uVar4;
  bVar7 = uVar4 == 0;
code_r0x000447a6:
  if (!bVar7) {
code_r0x000447aa:
    return uVar4;
  }
LAB_000447ac:
  *(undefined1 *)(unaff_A6 + 0x81) = 0;
  return uVar4;
}
